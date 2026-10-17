/* Texture replacement packs (upscale mods) in PCSX2's format, for the native renderer
   (DHD_RENDER=native; finding 057). Existing PCSX2 packs for this disc work unchanged: a pack is
   a folder of PNGs named by PCSX2's texture hash, textures\SCUS-97177\replacements in PCSX2.

   Names (PCSX2 pcsx2/GS/Renderers/HW/GSTextureReplacements.cpp and GSTextureCache.cpp,
   GPL-3.0-or-later, PCSX2 Dev Team; the scheme is reimplemented here):
     <TEX0 hash>-<CLUT hash>-<bits>.png   paletted formats (PSMT8, T4, T8H, T4HL, T4HH)
     <TEX0 hash>-<bits>.png               the others
   - TEX0 hash: XXH3-64 of the texture's GS memory. Where the format fills its 32-bit words
     (PSMCT32, PSMT8, PSMT4, PSMZ32) and the texture is at least a block, the raw 256-byte blocks
     it covers, row by row of blocks. Otherwise its texels expanded, row by row: an index byte
     each for the paletted formats, a 32-bit colour (TEXA applied) for the direct ones. A draw
     that uses mipmaps (MXL > 0, a mipmapped MMIN) also hashes levels 1..MXL, from MIPTBP1/2.
   - CLUT hash: XXH3-64 of the palette as 32-bit colours (16 or 256), CSA applied.
   - bits: PSM | TW << 6 | TH << 10, plus TEXA's TA0 << 15 | AEM << 23 | TA1 << 24 for the
     16- and 24-bit direct formats. Bit 14 was TCC in older PCSX2 versions and is ignored.
   Other names (regions "-rWxH-", mip levels "-mipN") are not ones PCSX2 loads; they are skipped.

   PNG alpha is GS alpha, as in PCSX2: 128 is opaque (a PNG without alpha gets 128). The
   replacement is sampled over the same normalised texture space as the original (raster.hlsl,
   flag 2048), with mipmaps made here, so a 4x pack draws 4x the detail.

   Loading: the folder's file names are indexed at start. A texture's replacement is decoded
   (WIC, part of Windows) and uploaded on loader threads the first time it is drawn; until then
   the original is drawn. Uploaded replacements count against a memory budget (DHD_TEXTURE_
   CACHE_MB, dhd.ini texture_cache_mb; default half the graphics card's memory, 512-2048 MB);
   over it, those no field drew lately are released and loaded again when needed. */
#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wincodec.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "texpack.h"

#define XXH_INLINE_ALL
#define XXH_STATIC_LINKING_ONLY
#include "xxhash.h"   /* xxHash 0.8.3, BSD-2-Clause (THIRD-PARTY-NOTICES.txt) */

void host_thread_name(const char *name);  /* port/hle/host.c */

enum { PACK_IDLE, PACK_QUEUED, PACK_READY, PACK_FAILED };
enum { PACK_LOADERS = 2, PACK_MAX_SIDE = 4096 };

struct TexPackEntry {
    uint64_t tex0_hash, clut_hash;
    uint32_t bits;
    wchar_t *path;
    volatile LONG state;
    ID3D11Texture2D *texture;          /* loader thread, before state becomes READY */
    ID3D11ShaderResourceView *view;
    uint32_t bytes;
    uint64_t used;                     /* GS worker: the last field that drew it */
};

static struct {
    ID3D11Device *device;
    TexPackEntry *entries;
    unsigned count, capacity;
    int32_t *table;                    /* open addressing, entry index or -1 */
    unsigned mask;
    wchar_t root[MAX_PATH];
    unsigned root_length;
    unsigned skipped_names;
    /* loading */
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE wake;
    TexPackEntry **queue;              /* a ring one longer than the index: each entry is queued once at most */
    unsigned head, tail;
    volatile LONG64 bytes;
    uint64_t budget, peak;
    volatile LONG loaded, failed;
    volatile LONG64 load_us;
    uint64_t evicted, last_evict;
    /* GS worker */
    uint8_t *scratch;
    size_t scratch_size;
    /* counts: distinct names seen, and found */
    uint64_t *seen;                    /* open addressing, 0 empty */
    unsigned seen_mask, seen_count, seen_hits, seen_mips;
    unsigned psm_seen[64], psm_hits[64];
    FILE *log;
} pack;

/* ---- the index ---- */

static uint64_t pack_key(uint64_t tex0_hash, uint64_t clut_hash, uint32_t bits)
{
    uint64_t h = tex0_hash ^ (clut_hash * 0x9E3779B97F4A7C15ull) ^ ((uint64_t)bits * 0xC2B2AE3D27D4EB4Full);
    h ^= h >> 29; h *= 0xBF58476D1CE4E5B9ull; h ^= h >> 32;
    return h;
}

static TexPackEntry *pack_lookup(uint64_t tex0_hash, uint64_t clut_hash, uint32_t bits)
{
    for (unsigned i = (unsigned)pack_key(tex0_hash, clut_hash, bits) & pack.mask;; i = (i + 1) & pack.mask) {
        int32_t at = pack.table[i];
        if (at < 0) return NULL;
        TexPackEntry *e = &pack.entries[at];
        if (e->tex0_hash == tex0_hash && e->clut_hash == clut_hash && e->bits == bits) return e;
    }
}

static int hex_field(const wchar_t *s, size_t length, uint64_t *out)
{
    uint64_t v = 0;
    if (!length || length > 16) return 0;
    for (size_t i = 0; i < length; i++) {
        wchar_t c = s[i];
        unsigned d = c >= L'0' && c <= L'9' ? c - L'0' : c >= L'a' && c <= L'f' ? c - L'a' + 10 :
                     c >= L'A' && c <= L'F' ? c - L'A' + 10 : 16;
        if (d == 16) return 0;
        v = v << 4 | d;
    }
    *out = v;
    return 1;
}

/* "<hash>-<clut>-<bits>.png" or "<hash>-<bits>.png" (PCSX2's ParseReplacementName, without
   the region forms). */
static int pack_parse(const wchar_t *name, uint64_t *tex0_hash, uint64_t *clut_hash, uint32_t *bits)
{
    size_t length = wcslen(name);
    if (length < 5 || _wcsicmp(name + length - 4, L".png")) return 0;
    length -= 4;
    const wchar_t *part[3];
    size_t size[3], parts = 0, start = 0;
    for (size_t i = 0; i <= length; i++)
        if (i == length || name[i] == L'-') {
            if (parts == 3) return 0;
            part[parts] = name + start; size[parts++] = i - start;
            start = i + 1;
        }
    uint64_t v[3];
    for (size_t i = 0; i < parts; i++)
        if (!hex_field(part[i], size[i], &v[i])) return 0;
    if (parts < 2 || size[parts - 1] > 8) return 0;
    *tex0_hash = v[0];
    *clut_hash = parts == 3 ? v[1] : 0;
    *bits = (uint32_t)v[parts - 1] & ~(1u << 14);
    return 1;
}

static void pack_add(const wchar_t *relative, const wchar_t *name)
{
    uint64_t tex0_hash, clut_hash;
    uint32_t bits;
    if (!pack_parse(name, &tex0_hash, &clut_hash, &bits)) { pack.skipped_names++; return; }
    if (pack.count == pack.capacity) {
        unsigned capacity = pack.capacity ? pack.capacity * 2 : 4096;
        TexPackEntry *grown = realloc(pack.entries, capacity * sizeof *grown);
        if (!grown) return;
        pack.entries = grown; pack.capacity = capacity;
    }
    size_t length = wcslen(relative);
    wchar_t *path = malloc((length + 1) * sizeof *path);
    if (!path) return;
    memcpy(path, relative, (length + 1) * sizeof *path);
    TexPackEntry *e = &pack.entries[pack.count++];
    memset(e, 0, sizeof *e);
    e->tex0_hash = tex0_hash; e->clut_hash = clut_hash; e->bits = bits; e->path = path;
}

/* Every PNG below the folder (PCSX2 searches it recursively too). */
static void pack_scan(wchar_t *relative, size_t length, int depth)
{
    wchar_t pattern[MAX_PATH * 2];
    WIN32_FIND_DATAW found;
    if (_snwprintf(pattern, MAX_PATH * 2, L"%ls\\%ls*", pack.root, relative) < 0) return;
    HANDLE h = FindFirstFileExW(pattern, FindExInfoBasic, &found, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        size_t name = wcslen(found.cFileName);
        if (found.cFileName[0] == L'.' || length + name + 2 >= MAX_PATH) continue;
        memcpy(relative + length, found.cFileName, (name + 1) * sizeof *relative);
        if (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (depth < 8) {
                relative[length + name] = L'\\'; relative[length + name + 1] = 0;
                pack_scan(relative, length + name + 1, depth + 1);
            }
        } else pack_add(relative, found.cFileName);
        relative[length] = 0;
    } while (FindNextFileW(h, &found));
    FindClose(h);
}

static int pack_folder(const wchar_t *path)
{
    DWORD a = GetFileAttributesW(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

/* The pack's folder: DHD_TEXTURE_PACK (the replacements folder, or a folder holding
   replacements\ or SCUS-97177\replacements\, as PCSX2's textures folder does), else
   textures\SCUS-97177\replacements beside dhd.exe. */
static int pack_find_root(wchar_t root[MAX_PATH], int quiet)
{
    wchar_t base[MAX_PATH], tried[MAX_PATH];
    static const wchar_t *const below[] = {L"", L"\\replacements", L"\\SCUS-97177\\replacements"};
    const char *named = getenv("DHD_TEXTURE_PACK");
    if (named && *named) {
        if (!MultiByteToWideChar(CP_ACP, 0, named, -1, base, MAX_PATH)) return 0;
        size_t n = wcslen(base);
        while (n > 3 && (base[n - 1] == L'\\' || base[n - 1] == L'/')) base[--n] = 0;
        for (int i = 2; i >= 0; i--) {
            _snwprintf(tried, MAX_PATH, L"%ls%ls", base, below[i]);
            tried[MAX_PATH - 1] = 0;
            if (pack_folder(tried)) { wcscpy(root, tried); return 1; }
        }
        if (!quiet) fprintf(stderr, "textures: pack folder %s not found (texture_pack in dhd.ini)\n", named);
        return 0;
    }
    DWORD n = GetModuleFileNameW(NULL, base, MAX_PATH);
    while (n && base[n - 1] != L'\\' && base[n - 1] != L'/') n--;
    base[n] = 0;
    _snwprintf(tried, MAX_PATH, L"%lstextures\\SCUS-97177\\replacements", base);
    tried[MAX_PATH - 1] = 0;
    if (!pack_folder(tried)) return 0;
    wcscpy(root, tried);
    return 1;
}

int texpack_available(void)
{
    wchar_t root[MAX_PATH];
    return pack_find_root(root, 1);
}

/* ---- loading (loader threads) ---- */

static const GUID pack_clsid_wic = {0xcacaf262, 0x9370, 0x4615, {0xa1, 0x3b, 0x9f, 0x55, 0x39, 0xda, 0x4c, 0x0a}};
static const GUID pack_iid_wic = {0xec5ec8a9, 0xc395, 0x4314, {0x9c, 0x77, 0x54, 0xd7, 0xa9, 0x35, 0xff, 0x70}};
static const GUID pack_iid_format_info2 = {0xa9db33a2, 0xaf5f, 0x43c7, {0xb6, 0x79, 0x74, 0xf5, 0x98, 0x4b, 0x5a, 0xa4}};
static const GUID pack_format_rgba = {0xf5c7ad2d, 0x6a8d, 0x43dd, {0xa7, 0xa8, 0xa2, 0x99, 0x35, 0x26, 0x1a, 0xe9}};

static int pack_has_alpha(IWICImagingFactory *wic, const GUID *format)
{
    IWICComponentInfo *info = NULL;
    IWICPixelFormatInfo2 *pixel = NULL;
    BOOL alpha = TRUE;
    if (SUCCEEDED(IWICImagingFactory_CreateComponentInfo(wic, format, &info))) {
        if (SUCCEEDED(IWICComponentInfo_QueryInterface(info, &pack_iid_format_info2, (void **)&pixel))) {
            IWICPixelFormatInfo2_SupportsTransparency(pixel, &alpha);
            IWICPixelFormatInfo2_Release(pixel);
        }
        IWICComponentInfo_Release(info);
    }
    return alpha != FALSE;
}

/* The next level from this one, a 2x2 box (an odd side keeps its last texel). */
static void pack_halve(const uint8_t *from, unsigned w, unsigned h, uint8_t *to)
{
    unsigned nw = w > 1 ? w / 2 : 1, nh = h > 1 ? h / 2 : 1;
    for (unsigned y = 0; y < nh; y++) {
        const uint8_t *r0 = from + (size_t)(y * 2 < h ? y * 2 : h - 1) * w * 4;
        const uint8_t *r1 = from + (size_t)(y * 2 + 1 < h ? y * 2 + 1 : h - 1) * w * 4;
        for (unsigned x = 0; x < nw; x++) {
            unsigned x0 = x * 2 < w ? x * 2 : w - 1, x1 = x * 2 + 1 < w ? x * 2 + 1 : w - 1;
            for (unsigned c = 0; c < 4; c++)
                to[((size_t)y * nw + x) * 4 + c] =
                    (uint8_t)((r0[x0 * 4 + c] + r0[x1 * 4 + c] + r1[x0 * 4 + c] + r1[x1 * 4 + c] + 2) >> 2);
        }
    }
}

static int pack_load(IWICImagingFactory *wic, TexPackEntry *e)
{
    wchar_t path[MAX_PATH * 2];
    IWICBitmapDecoder *decoder = NULL;
    IWICBitmapFrameDecode *frame = NULL;
    IWICFormatConverter *converter = NULL;
    uint8_t *pixels = NULL;
    UINT w = 0, h = 0;
    int ok = 0;
    _snwprintf(path, MAX_PATH * 2, L"%ls\\%ls", pack.root, e->path);
    path[MAX_PATH * 2 - 1] = 0;
    if (FAILED(IWICImagingFactory_CreateDecoderFromFilename(wic, path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand,
                                                            &decoder)))
        goto done;
    if (FAILED(IWICBitmapDecoder_GetFrame(decoder, 0, &frame))) goto done;
    if (FAILED(IWICBitmapFrameDecode_GetSize(frame, &w, &h)) || !w || !h || w > PACK_MAX_SIDE || h > PACK_MAX_SIDE)
        goto done;
    WICPixelFormatGUID format;
    if (FAILED(IWICBitmapFrameDecode_GetPixelFormat(frame, &format))) goto done;
    int alpha = pack_has_alpha(wic, &format);
    if (FAILED(IWICImagingFactory_CreateFormatConverter(wic, &converter))) goto done;
    if (FAILED(IWICFormatConverter_Initialize(converter, (IWICBitmapSource *)frame, &pack_format_rgba,
                                              WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom)))
        goto done;
    /* All the levels in one buffer, down to 1x1. */
    unsigned levels = 1;
    size_t total = (size_t)w * h * 4;
    for (unsigned lw = w, lh = h; lw > 1 || lh > 1; levels++) {
        lw = lw > 1 ? lw / 2 : 1; lh = lh > 1 ? lh / 2 : 1;
        total += (size_t)lw * lh * 4;
    }
    if (!(pixels = malloc(total))) goto done;
    if (FAILED(IWICFormatConverter_CopyPixels(converter, NULL, w * 4, w * h * 4, pixels))) goto done;
    if (!alpha)
        for (size_t i = 3; i < (size_t)w * h * 4; i += 4) pixels[i] = 0x80;  /* PCSX2: opaque in GS units */
    D3D11_SUBRESOURCE_DATA data[16];
    uint8_t *level = pixels;
    unsigned lw = w, lh = h;
    for (unsigned i = 0; i < levels; i++) {
        data[i].pSysMem = level; data[i].SysMemPitch = lw * 4; data[i].SysMemSlicePitch = 0;
        if (i + 1 < levels) {
            uint8_t *next = level + (size_t)lw * lh * 4;
            pack_halve(level, lw, lh, next);
            level = next;
            lw = lw > 1 ? lw / 2 : 1; lh = lh > 1 ? lh / 2 : 1;
        }
    }
    D3D11_TEXTURE2D_DESC d = {0};
    d.Width = w; d.Height = h; d.MipLevels = levels; d.ArraySize = 1; d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    d.SampleDesc.Count = 1; d.Usage = D3D11_USAGE_IMMUTABLE; d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    if (FAILED(ID3D11Device_CreateTexture2D(pack.device, &d, data, &e->texture))) { e->texture = NULL; goto done; }
    if (FAILED(ID3D11Device_CreateShaderResourceView(pack.device, (ID3D11Resource *)e->texture, NULL, &e->view))) {
        ID3D11Texture2D_Release(e->texture); e->texture = NULL; e->view = NULL;
        goto done;
    }
    e->bytes = (uint32_t)total;
    ok = 1;
done:
    free(pixels);
    if (converter) IWICFormatConverter_Release(converter);
    if (frame) IWICBitmapFrameDecode_Release(frame);
    if (decoder) IWICBitmapDecoder_Release(decoder);
    return ok;
}

static unsigned __stdcall pack_loader(void *argument)
{
    (void)argument;
    host_thread_name("texpack");
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
    IWICImagingFactory *wic = NULL;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(CoCreateInstance(&pack_clsid_wic, NULL, CLSCTX_INPROC_SERVER, &pack_iid_wic, (void **)&wic))) {
        fprintf(stderr, "textures: WIC unavailable; no replacements load\n");
        return 0;
    }
    for (;;) {
        EnterCriticalSection(&pack.lock);
        while (pack.head == pack.tail) SleepConditionVariableCS(&pack.wake, &pack.lock, INFINITE);
        TexPackEntry *e = pack.queue[pack.head];
        pack.head = (pack.head + 1) % (pack.count + 1);
        LeaveCriticalSection(&pack.lock);
        LARGE_INTEGER t0, t1, f;
        QueryPerformanceCounter(&t0);
        int ok = pack_load(wic, e);
        QueryPerformanceCounter(&t1);
        QueryPerformanceFrequency(&f);
        InterlockedExchangeAdd64(&pack.load_us, (LONG64)((t1.QuadPart - t0.QuadPart) * 1000000 / f.QuadPart));
        if (ok) {
            InterlockedExchangeAdd64(&pack.bytes, e->bytes);
            InterlockedIncrement(&pack.loaded);
        } else {
            InterlockedIncrement(&pack.failed);
            char narrow[MAX_PATH];
            WideCharToMultiByte(CP_UTF8, 0, e->path, -1, narrow, MAX_PATH, NULL, NULL);
            fprintf(stderr, "textures: can't load %s\n", narrow);
        }
        InterlockedExchange(&e->state, ok ? PACK_READY : PACK_FAILED);  /* a full barrier */
    }
}

/* ---- opening ---- */

static uint64_t pack_default_budget(ID3D11Device *device)
{
    static const GUID dxgi_device_iid = {0x54ec77fa, 0x1377, 0x44e6, {0x8c, 0x32, 0x88, 0xfd, 0x5f, 0x44, 0xc8, 0x4c}};
    uint64_t budget = 1024ull << 20;
    IDXGIDevice *dxgi = NULL;
    IDXGIAdapter *adapter = NULL;
    DXGI_ADAPTER_DESC desc;
    if (SUCCEEDED(ID3D11Device_QueryInterface(device, &dxgi_device_iid, (void **)&dxgi))) {
        if (SUCCEEDED(IDXGIDevice_GetAdapter(dxgi, &adapter))) {
            if (SUCCEEDED(IDXGIAdapter_GetDesc(adapter, &desc)) && desc.DedicatedVideoMemory)
                budget = (uint64_t)desc.DedicatedVideoMemory / 2;
            IDXGIAdapter_Release(adapter);
        }
        IDXGIDevice_Release(dxgi);
    }
    if (budget < (512ull << 20)) budget = 512ull << 20;
    if (budget > (2048ull << 20)) budget = 2048ull << 20;  /* a 32-bit process */
    return budget;
}

static int pack_switch(const char *v)
{
    return !v || !*v || !(!_stricmp(v, "off") || !_stricmp(v, "0") || !_stricmp(v, "false") || !_stricmp(v, "no"));
}

int texpack_open(ID3D11Device *device)
{
    if (!pack_switch(getenv("DHD_TEXTURES"))) return 0;
    if (!pack_find_root(pack.root, 0)) return 0;
    LARGE_INTEGER t0, t1, f;
    QueryPerformanceCounter(&t0);
    wchar_t relative[MAX_PATH] = L"";
    pack.root_length = (unsigned)wcslen(pack.root);
    pack_scan(relative, 0, 0);
    QueryPerformanceCounter(&t1);
    QueryPerformanceFrequency(&f);
    char narrow[MAX_PATH * 3];
    WideCharToMultiByte(CP_UTF8, 0, pack.root, -1, narrow, sizeof narrow, NULL, NULL);
    if (!pack.count) {
        fprintf(stderr, "textures: no replacement textures in %s\n", narrow);
        return 0;
    }
    unsigned size = 1;
    while (size < pack.count * 2) size <<= 1;
    pack.table = malloc(size * sizeof *pack.table);
    pack.queue = malloc((pack.count + 1) * sizeof *pack.queue);
    pack.seen_mask = (1u << 16) - 1;
    pack.seen = calloc(pack.seen_mask + 1, sizeof *pack.seen);
    if (!pack.table || !pack.queue || !pack.seen) return 0;
    pack.mask = size - 1;
    memset(pack.table, 0xFF, size * sizeof *pack.table);
    unsigned duplicates = 0;
    for (unsigned i = 0; i < pack.count; i++) {
        TexPackEntry *e = &pack.entries[i];
        if (pack_lookup(e->tex0_hash, e->clut_hash, e->bits)) { duplicates++; continue; }
        unsigned at = (unsigned)pack_key(e->tex0_hash, e->clut_hash, e->bits) & pack.mask;
        while (pack.table[at] >= 0) at = (at + 1) & pack.mask;
        pack.table[at] = (int32_t)i;
    }
    pack.device = device;
    ID3D11Device_AddRef(device);
    const char *mb = getenv("DHD_TEXTURE_CACHE_MB");
    pack.budget = mb && atoi(mb) > 0 ? (uint64_t)atoi(mb) << 20 : pack_default_budget(device);
    InitializeCriticalSection(&pack.lock);
    InitializeConditionVariable(&pack.wake);
    for (unsigned i = 0; i < PACK_LOADERS; i++) {
        HANDLE t = (HANDLE)_beginthreadex(NULL, 0, pack_loader, NULL, 0, NULL);
        if (t) CloseHandle(t);
    }
    const char *log = getenv("DHD_TEXTURE_PACK_LOG");
    if (log && *log && (pack.log = fopen(log, "w")))
        fputs("field,psm,width,height,name,found,with_mips\n", pack.log);
    fprintf(stderr, "textures: %u replacements in %s (%u other files skipped, %u duplicates), indexed in %.0f ms, "
            "cache %llu MB\n", pack.count - duplicates, narrow, pack.skipped_names, duplicates,
            (double)(t1.QuadPart - t0.QuadPart) * 1000 / (double)f.QuadPart, (unsigned long long)(pack.budget >> 20));
    return 1;
}

/* ---- names (GS worker) ---- */

static unsigned pack_palette_size(unsigned psm)
{
    return psm == GS_PSMT8 || psm == GS_PSMT8H ? 256 : psm == GS_PSMT4 || psm == GS_PSMT4HL || psm == GS_PSMT4HH ? 16 : 0;
}

/* GSLocalMemory's block size (bs) and whether the format's bits fill each word (fmsk). */
static void pack_block(unsigned psm, unsigned *bw, unsigned *bh, int *whole)
{
    *bw = psm == GS_PSMT8 || (psm & 7) == 2 ? 16 : psm == GS_PSMT4 ? 32 : 8;
    *bh = psm == GS_PSMT8 || psm == GS_PSMT4 ? 16 : 8;
    *whole = psm == GS_PSMCT32 || psm == GS_PSMT8 || psm == GS_PSMT4 || psm == GS_PSMZ32;
}

static uint8_t *pack_room(size_t used, size_t more)
{
    if (used + more > pack.scratch_size) {
        size_t size = pack.scratch_size ? pack.scratch_size : 1 << 20;
        while (size < used + more) size *= 2;
        uint8_t *grown = realloc(pack.scratch, size);
        if (!grown) return NULL;
        pack.scratch = grown; pack.scratch_size = size;
    }
    return pack.scratch + used;
}

/* What PCSX2 hashes for one level (HashTextureLevel, no region): appended at pack.scratch + used. */
static size_t pack_level(const Gs *g, unsigned psm, unsigned tbp, unsigned tbw, unsigned tw, unsigned th, uint64_t texa,
                         size_t used)
{
    unsigned w = 1u << tw, h = 1u << th, bw, bh;
    int whole;
    pack_block(psm, &bw, &bh, &whole);
    const uint8_t *vram = (const uint8_t *)g->vram;
    if (whole && w >= bw && h >= bh) {
        uint8_t *out = pack_room(used, (size_t)(w / bw) * (h / bh) * 256);
        if (!out) return 0;
        for (unsigned y = 0; y < h; y += bh)
            for (unsigned x = 0; x < w; x += bw, out += 256)
                memcpy(out, vram + ((gs_pixel_address(psm, tbp, tbw, x, y) >> 11) & 0x3FFF) * 256, 256);
        return (size_t)(w / bw) * (h / bh) * 256;
    }
    int indexed = pack_palette_size(psm) != 0;
    size_t bytes = (size_t)w * h * (indexed ? 1 : 4);
    uint8_t *out = pack_room(used, bytes);
    if (!out) return 0;
    for (unsigned y = 0; y < h; y++)
        for (unsigned x = 0; x < w; x++) {
            uint32_t v = gs_image_pixel(g->vram, psm, tbp, tbw, x, y);
            if (indexed) *out++ = (uint8_t)v;
            else { v = gs_color(v, psm, texa); memcpy(out, &v, 4); out += 4; }
        }
    return bytes;
}

/* GSClut's 32-bit palette (Read32): CT32/CT24 entries as stored, CT16 expanded with TEXA. */
static uint64_t pack_clut_hash(const Gs *g, uint64_t tex0, unsigned entries)
{
    uint32_t palette[256];
    unsigned cpsm = (unsigned)(tex0 >> 51) & 15, csa = (unsigned)(tex0 >> 56) & 31;
    uint64_t texa = g->reg[GS_TEXA];
    for (unsigned i = 0; i < entries; i++) {
        if (cpsm & 2) {
            unsigned c = g->clut[(csa * 16 + i) & 511];
            unsigned alpha = c & 0x8000 ? (unsigned)(texa >> 32) & 255 : (unsigned)texa & 255;
            if ((texa & 0x8000) && !c) alpha = 0;
            palette[i] = (c & 31) << 3 | (c & 0x3E0) << 6 | (c & 0x7C00) << 9 | alpha << 24;
        } else {
            unsigned group = (entries == 16 ? (csa & 15) * 16 : (csa & 15) * 16 + (i & ~15u));
            unsigned at = (entries == 16 ? group : group > 240 ? 240 : group) + (i & 15);
            palette[i] = g->clut[at & 255] | (uint32_t)g->clut[(at & 255) + 256] << 16;
        }
    }
    return XXH3_64bits(palette, entries * 4);
}

static void pack_count(unsigned psm, unsigned tw, unsigned th, uint64_t tex0_hash, uint64_t clut_hash, uint32_t bits,
                       int found, int mips, uint64_t field)
{
    uint64_t key = pack_key(tex0_hash, clut_hash, bits) | 1;
    unsigned i = (unsigned)(key >> 32) & pack.seen_mask;
    while (pack.seen[i] && pack.seen[i] != key) i = (i + 1) & pack.seen_mask;
    if (pack.seen[i] || pack.seen_count >= pack.seen_mask / 2) return;
    pack.seen[i] = key;
    pack.seen_count++;
    pack.psm_seen[psm & 63]++;
    if (found) { pack.seen_hits++; pack.psm_hits[psm & 63]++; pack.seen_mips += mips; }
    if (pack.log) {
        if (pack_palette_size(psm))
            fprintf(pack.log, "%llu,%u,%u,%u,%llx-%llx-%08x,%d,%d\n", (unsigned long long)field, psm, 1u << tw,
                    1u << th, (unsigned long long)tex0_hash, (unsigned long long)clut_hash, bits, found, mips);
        else
            fprintf(pack.log, "%llu,%u,%u,%u,%llx-%08x,%d,%d\n", (unsigned long long)field, psm, 1u << tw, 1u << th,
                    (unsigned long long)tex0_hash, bits, found, mips);
    }
}

static uint64_t pack_field;

TexPackEntry *texpack_find(const Gs *g, unsigned ctxt)
{
    uint64_t tex0 = g->reg[GS_TEX0_1 + ctxt], tex1 = g->reg[GS_TEX1_1 + ctxt], texa = g->reg[GS_TEXA];
    unsigned psm = (unsigned)(tex0 >> 20) & 63, tbp = (unsigned)tex0 & 0x3FFF, tbw = (unsigned)(tex0 >> 14) & 63;
    unsigned tw = (unsigned)(tex0 >> 26) & 15, th = (unsigned)(tex0 >> 30) & 15;
    if (tw > 10 || th > 10) return NULL;
    unsigned entries = pack_palette_size(psm);
    uint32_t bits = psm | tw << 6 | th << 10, plain = bits;
    if (!entries && (psm & 7) != 0)  /* the 16- and 24-bit direct formats: TEXA is part of the name */
        bits |= ((uint32_t)texa & 255) << 15 | ((uint32_t)(texa >> 15) & 1) << 23 | ((uint32_t)(texa >> 32) & 255) << 24;
    uint64_t clut_hash = entries ? pack_clut_hash(g, tex0, entries) : 0;
    size_t used = pack_level(g, psm, tbp, tbw, tw, th, texa, 0);
    if (!used) return NULL;
    uint64_t base_hash = XXH3_64bits(pack.scratch, used);
    /* A mipmapped draw: PCSX2 (full mipmapping) hashes levels 1..MXL after the base. */
    unsigned mxl = (unsigned)(tex1 >> 2) & 7, mmin = (unsigned)(tex1 >> 6) & 7;
    uint64_t mip_hash = 0;
    int mipmapped = mxl > 0 && mmin >= 2 && mmin <= 5;
    if (mipmapped) {
        uint64_t m1 = g->reg[GS_MIPTBP1_1 + ctxt], m2 = g->reg[GS_MIPTBP2_1 + ctxt];
        if (mxl > 6) mxl = 6;
        size_t all = used;
        for (unsigned i = 1; i <= mxl; i++) {
            uint64_t m = i <= 3 ? m1 >> ((i - 1) * 20) : m2 >> ((i - 4) * 20);
            size_t more = pack_level(g, psm, (unsigned)m & 0x3FFF, (unsigned)(m >> 14) & 63, tw > i ? tw - i : 0,
                                     th > i ? th - i : 0, texa, all);
            if (!more) { mipmapped = 0; break; }
            all += more;
        }
        if (mipmapped) mip_hash = XXH3_64bits(pack.scratch, all);
    }
    TexPackEntry *e = NULL;
    int with_mips = 0;
    if (mipmapped && (e = pack_lookup(mip_hash, clut_hash, bits))) with_mips = 1;
    if (!e) e = pack_lookup(base_hash, clut_hash, bits);
    if (!e && plain != bits) {  /* packs dumped without TEXA in the name */
        if (mipmapped && (e = pack_lookup(mip_hash, clut_hash, plain))) with_mips = 1;
        if (!e) e = pack_lookup(base_hash, clut_hash, plain);
    }
    pack_count(psm, tw, th, mipmapped ? mip_hash : base_hash, clut_hash, bits, e != NULL, with_mips, pack_field);
    return e;
}

/* ---- use (GS worker) ---- */

static uint64_t pack_bytes(void) { return (uint64_t)InterlockedCompareExchange64(&pack.bytes, 0, 0); }

static void pack_evict(uint64_t field)
{
    /* The least recently drawn first, while over the budget; never one a queued field may draw. */
    while (pack_bytes() > pack.budget) {
        TexPackEntry *oldest = NULL;
        for (unsigned i = 0; i < pack.count; i++) {
            TexPackEntry *e = &pack.entries[i];
            if (e->state == PACK_READY && e->used + 8 < field && (!oldest || e->used < oldest->used)) oldest = e;
        }
        if (!oldest) return;
        ID3D11ShaderResourceView_Release(oldest->view);
        ID3D11Texture2D_Release(oldest->texture);
        oldest->view = NULL; oldest->texture = NULL;
        InterlockedExchangeAdd64(&pack.bytes, -(LONG64)oldest->bytes);
        oldest->state = PACK_IDLE;
        pack.evicted++;
    }
}

ID3D11ShaderResourceView *texpack_view(TexPackEntry *e, uint64_t field)
{
    LONG state = e->state;
    if (state == PACK_READY) {
        e->used = field;
        return e->view;
    }
    if (state != PACK_IDLE) return NULL;
    if (pack_bytes() > pack.budget) {
        if (pack.last_evict == field) return NULL;
        pack.last_evict = field;
        pack_evict(field);
        if (pack_bytes() > pack.budget) return NULL;
    }
    e->used = field;
    EnterCriticalSection(&pack.lock);
    e->state = PACK_QUEUED;
    pack.queue[pack.tail] = e;
    pack.tail = (pack.tail + 1) % (pack.count + 1);
    WakeConditionVariable(&pack.wake);
    LeaveCriticalSection(&pack.lock);
    return NULL;
}

void texpack_field(uint64_t field)
{
    pack_field = field;
    uint64_t bytes = pack_bytes();
    if (bytes > pack.peak) pack.peak = bytes;
    if (bytes > pack.budget && field % 30 == 0 && pack.last_evict != field) {
        pack.last_evict = field;
        pack_evict(field);
    }
    if (pack.log && field % 600 == 0) fflush(pack.log);
}

void texpack_report(void)
{
    if (!pack.count) return;
    fprintf(stderr, "textures: %u distinct textures drawn, %u in the pack (%.1f%%; %u named with mip levels), "
            "%ld loaded (%.1f ms each), %ld failed, %llu released, peak %llu MB of %llu MB\n",
            pack.seen_count, pack.seen_hits, pack.seen_count ? 100.0 * pack.seen_hits / pack.seen_count : 0.0,
            pack.seen_mips, pack.loaded, pack.loaded ? (double)pack.load_us / 1000 / pack.loaded : 0.0, pack.failed,
            (unsigned long long)pack.evicted, (unsigned long long)(pack.peak >> 20),
            (unsigned long long)(pack.budget >> 20));
    for (unsigned psm = 0; psm < 64; psm++)
        if (pack.psm_seen[psm])
            fprintf(stderr, "textures: psm %u: %u drawn, %u in the pack\n", psm, pack.psm_seen[psm], pack.psm_hits[psm]);
    if (pack.log) fflush(pack.log);
}
