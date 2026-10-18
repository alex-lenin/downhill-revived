/* Ordered compute draws use GS VRAM directly, including framebuffer textures.
   This is a correctness-first backend: no texture cache or render-target aliases
   can go stale. CPU transfers/readback synchronize through the same 4 MB image. */
#define COBJMACROS
#include "gpu_win32.h"
#include <d3d11_3.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gs_shader.h"  /* embedded from raster.hlsl by the build */

typedef struct {
    uint32_t reg[128][4], position[3][4], uv[3][4];
    float stq[3][4];
    uint32_t bounds[4], info[4], palette[256];
} Constants;

enum { MAX_DRAWS = 256, MAX_BIN_WORDS = 1024 * 1024 };
typedef struct {
    uint32_t position[3][4], uv[3][4];
    float stq[3][4];
    uint32_t info[4], bounds[4];
    uint32_t depth[8];
} Primitive;

enum { TRACE_RING = 32, TRACE_DISPATCHES = 2048 };
typedef struct {
    ID3D11Query *disjoint, *stamp[4], *dispatch[TRACE_DISPATCHES * 2];
    uint64_t cpu_begin, cpu_present_begin, cpu_end, primitives, upload_bytes;
    unsigned frame, pending, count, overflow, raster_count, scanout;
    unsigned char hardware[TRACE_DISPATCHES], reason[TRACE_DISPATCHES];
    unsigned char primitive_kind[TRACE_DISPATCHES], texture_kind[TRACE_DISPATCHES];
    double wall_end;
} GpuTraceFrame;
typedef struct {
    ID3D11Device *device;
    ID3D11DeviceContext *context;
    ID3D11Buffer *vram, *snapshot, *readback, *constants, *staged;
    ID3D11UnorderedAccessView *uav;
    ID3D11ShaderResourceView *srv;
    ID3D11ComputeShader *shader;
    ID3D11VertexShader *hardware_vertex;
    ID3D11PixelShader *hardware_pixel;
    ID3D11RasterizerState2 *hardware_rasterizer;
    ID3D11DepthStencilState *hardware_depth;
    GsGpuRasterStats raster_stats;
    unsigned hardware_rejection;
    ID3D11Buffer *primitives, *bins;
    ID3D11ShaderResourceView *primitive_srv, *bin_srv;
    Primitive pending[MAX_DRAWS];
    Constants state;
    uint32_t *bin_data;
    unsigned pending_count, memberships, dispatches, batch_limit;
    uint64_t uploaded;
    int dirty;
    int profile;
    FILE *trace;
    GpuTraceFrame trace_frame[TRACE_RING];
    unsigned trace_serial;
    unsigned trace_dispatch_hardware, trace_dispatch_rejection;
    uint64_t trace_primitives, trace_upload_bytes;
    unsigned trace_raster_count;
    LARGE_INTEGER trace_frequency;
    uint64_t draw_ticks, sync_ticks, draws, snapshots, uploads, copies;
    uint32_t cpu_dirty[16], texture_dirty[16], texture_pages[16];
    uint64_t texture_key, clamp_key, tex1_key, mip_key[2];
    int texture_valid, batch_safe;
    uint64_t state_version;
    unsigned state_context;
    ID3D11ComputeShader *display_shader, *copy_shader;
    ID3D11Buffer *display_constants, *copy_constants;
    ID3D11ShaderResourceView *vram_srv;
    ID3D11Texture2D *display, *display_readback;
    ID3D11UnorderedAccessView *display_uav;
    unsigned display_width, display_height;
    /* DHD_RENDER=native (gpu_native.inc): this backend then only answers the Z probe, and
       clips every primitive to clip (inclusive pixels). */
    struct Native *native;
    int clip_on, clip[4];
} Gpu;

/* raster.hlsl's shaders, compiled once and kept: D3DCompile takes seconds for each of them, at
   every start. The bytecode is kept in %LOCALAPPDATA%\DownhillRevived\shaders under a name made
   of everything the compile depends on: the source, the entry point, the target, the defines, the
   flags and the system's d3dcompiler_47.dll (its size and date). Without that folder (or with
   DHD_NO_SHADER_CACHE): compiled each time, as before. Same arguments as D3DCompile's. */
static uint64_t fnv(uint64_t h, const void *data, size_t size)
{
    const unsigned char *p = data;
    for (size_t i = 0; i < size; i++) h = (h ^ p[i]) * 0x100000001B3ull;
    return h;
}

static int shader_cache_path(char *out, size_t size, const D3D_SHADER_MACRO *defines, const char *entry,
                             const char *target, UINT flags)
{
    const char *base = getenv("LOCALAPPDATA");
    char dll[MAX_PATH];
    WIN32_FILE_ATTRIBUTE_DATA info;
    HMODULE compiler = GetModuleHandleA("d3dcompiler_47.dll");
    if (!base || !*base || getenv("DHD_NO_SHADER_CACHE") || !compiler || !GetModuleFileNameA(compiler, dll, sizeof dll) ||
        !GetFileAttributesExA(dll, GetFileExInfoStandard, &info))
        return 0;
    uint64_t h = 0xCBF29CE484222325ull;
    h = fnv(h, gs_shader, sizeof gs_shader - 1);
    h = fnv(h, entry, strlen(entry) + 1);
    h = fnv(h, target, strlen(target) + 1);
    for (const D3D_SHADER_MACRO *d = defines; d && d->Name; d++)
        h = fnv(fnv(h, d->Name, strlen(d->Name) + 1), d->Definition, strlen(d->Definition) + 1);
    h = fnv(h, &flags, sizeof flags);
    h = fnv(h, &info.nFileSizeLow, sizeof info.nFileSizeLow);
    h = fnv(h, &info.ftLastWriteTime, sizeof info.ftLastWriteTime);
    snprintf(out, size, "%s\\DownhillRevived", base);
    CreateDirectoryA(out, NULL);
    snprintf(out, size, "%s\\DownhillRevived\\shaders", base);
    CreateDirectoryA(out, NULL);
    snprintf(out, size, "%s\\DownhillRevived\\shaders\\%s-%016llx.cso", base, entry, (unsigned long long)h);
    return 1;
}

static HRESULT shader_compile(const D3D_SHADER_MACRO *defines, const char *entry, const char *target, UINT flags,
                              ID3DBlob **code, ID3DBlob **errors)
{
    char path[MAX_PATH];
    int cached = shader_cache_path(path, sizeof path, defines, entry, target, flags);
    *code = NULL;
    if (errors) *errors = NULL;
    if (cached) {
        FILE *f = fopen(path, "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            fseek(f, 0, SEEK_SET);
            if (size > 0 && SUCCEEDED(D3DCreateBlob((SIZE_T)size, code)) &&
                fread(ID3D10Blob_GetBufferPointer(*code), 1, (size_t)size, f) == (size_t)size) {
                fclose(f);
                return S_OK;
            }
            if (*code) ID3D10Blob_Release(*code);
            *code = NULL;
            fclose(f);
        }
    }
    HRESULT hr = D3DCompile(gs_shader, sizeof gs_shader - 1, gs_shader_path, defines, NULL, entry, target, flags, 0,
                            code, errors);
    if (SUCCEEDED(hr) && cached) {
        char temp[MAX_PATH + 16];
        snprintf(temp, sizeof temp, "%s.%lu", path, GetCurrentProcessId());
        FILE *f = fopen(temp, "wb");
        if (f) {
            size_t n = ID3D10Blob_GetBufferSize(*code);
            int ok = fwrite(ID3D10Blob_GetBufferPointer(*code), 1, n, f) == n;
            ok = fclose(f) == 0 && ok;
            if (!ok || !MoveFileExA(temp, path, MOVEFILE_REPLACE_EXISTING)) DeleteFileA(temp);
        }
    }
    return hr;
}

static void flush(Gpu *gpu);
static int native_capture(struct Native *n, uint32_t *pixels);
static void native_profile(struct Native *n);
#include "gpu_hw.inc"

static uint64_t ticks(void) { LARGE_INTEGER n; QueryPerformanceCounter(&n); return (uint64_t)n.QuadPart; }

/* Queries are polled only after four subsequent frames, without flush or wait.
   The render interval includes GPU starvation; dispatch sum excludes those gaps.
   Detailed dispatch timings sample one complete interval per 32 frames. */
static ID3D11Query *trace_query(Gpu *p, D3D11_QUERY kind)
{
    D3D11_QUERY_DESC d = {kind, 0}; ID3D11Query *q = NULL;
    if (FAILED(ID3D11Device_CreateQuery(p->device, &d, &q))) return NULL;
    return q;
}
static void trace_dispatch(Gpu *p, int end)
{
    if (!p->trace || p->trace_serial % TRACE_RING) return;
    GpuTraceFrame *f = &p->trace_frame[0];
    if (f->pending != 1) return;
    if (f->count >= TRACE_DISPATCHES) { if (end) f->overflow++; return; }
    ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)f->dispatch[f->count * 2 + end]);
    if (end) {
        unsigned at = f->count, kind = p->pending[0].info[0];
        f->hardware[at] = (unsigned char)p->trace_dispatch_hardware;
        f->reason[at] = (unsigned char)p->trace_dispatch_rejection;
        for (unsigned i = 1; i < p->pending_count; i++)
            if (p->pending[i].info[0] != kind) { kind = 7; break; }
        f->primitive_kind[at] = (unsigned char)(kind < 7 ? kind : 7);
        unsigned texture = 4; /* untextured; 3 denotes another texture format */
        if (p->pending[0].info[1] & 16) {
            unsigned psm = p->state.reg[GS_TEX0_1 + p->state_context][0] >> 20 & 63;
            texture = psm == 0 ? 0 : psm == 19 ? 1 : psm == 20 ? 2 : 3;
        }
        for (unsigned i = 1; i < p->pending_count; i++)
            if ((p->pending[i].info[1] ^ p->pending[0].info[1]) & 16) { texture = 3; break; }
        f->texture_kind[at] = (unsigned char)texture;
        f->count++;
    }
}
void gs_gpu_trace_present(Gs *g, int end, double wall)
{
    Gpu *p = g->backend;
    if (!p || !p->trace) return;
    GpuTraceFrame *f = &p->trace_frame[p->trace_serial % TRACE_RING];
    if (f->pending == 1) {
        if (!end) {
            if (!f->scanout) {
                ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)f->stamp[1]);
                ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)f->stamp[2]);
            }
            ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)f->stamp[3]);
            ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)f->disjoint);
            f->cpu_present_begin = ticks();
        } else {
            f->cpu_end = ticks(); f->pending = 2; f->wall_end = wall;
            f->raster_count = p->trace_raster_count;
            f->primitives = p->trace_primitives; f->upload_bytes = p->trace_upload_bytes;
        }
    }
    if (!end) return;
    p->trace_primitives = p->trace_upload_bytes = 0; p->trace_raster_count = 0;
    p->trace_serial++;
    for (unsigned i = 0; i < TRACE_RING; i++) {
        GpuTraceFrame *r = &p->trace_frame[i];
        if (r->pending != 2 || p->trace_serial - r->frame < 4) continue;
        D3D11_QUERY_DATA_TIMESTAMP_DISJOINT d; uint64_t t[4], a, b; double dispatch_ms = 0, hardware_ms = 0, compute_ms = 0;
        unsigned hardware_count = 0, compute_count = 0;
        unsigned reason_count[GS_GPU_HW_REASON_COUNT] = {0}, kind_count[8] = {0}, texture_count[5] = {0};
        double reason_ms[GS_GPU_HW_REASON_COUNT] = {0}, kind_ms[8] = {0}, texture_ms[5] = {0};
        if (ID3D11DeviceContext_GetData(p->context, (ID3D11Asynchronous *)r->disjoint, &d, sizeof d, D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK) continue;
        int ready = 1;
        for (unsigned j = 0; j < 4; j++)
            if (ID3D11DeviceContext_GetData(p->context, (ID3D11Asynchronous *)r->stamp[j], &t[j], sizeof t[j], D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK) ready = 0;
        for (unsigned j = 0; j < r->count && ready; j++) {
            if (ID3D11DeviceContext_GetData(p->context, (ID3D11Asynchronous *)r->dispatch[j*2], &a, sizeof a, D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK ||
                ID3D11DeviceContext_GetData(p->context, (ID3D11Asynchronous *)r->dispatch[j*2+1], &b, sizeof b, D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK) ready = 0;
            else if (d.Frequency) {
                double elapsed = (b-a)*1000.0/d.Frequency;
                dispatch_ms += elapsed;
                if (r->hardware[j]) {
                    hardware_ms += elapsed; hardware_count++;
                    unsigned t = r->texture_kind[j]; texture_count[t]++; texture_ms[t] += elapsed;
                } else {
                    compute_ms += elapsed; compute_count++;
                    unsigned reason = r->reason[j], kind = r->primitive_kind[j];
                    if (reason < GS_GPU_HW_REASON_COUNT) { reason_count[reason]++; reason_ms[reason] += elapsed; }
                    kind_count[kind]++; kind_ms[kind] += elapsed;
                }
            }
        }
        if (!ready) continue;
        double k = d.Frequency ? 1000.0/d.Frequency : 0;
        fprintf(p->trace, "%u,%.6f,%u,%llu,%llu,%llu,%d,%.6f,%.6f,%.6f,%.6f,%u,%u,%llu,%llu,%u,%.6f,%u,%.6f",
            r->frame, r->wall_end, r->raster_count, r->cpu_begin, r->cpu_present_begin, r->cpu_end, d.Disjoint || !d.Frequency,
            (t[1]-t[0])*k, (t[2]-t[1])*k, (t[3]-t[2])*k, dispatch_ms,
            r->count, r->overflow, r->primitives, r->upload_bytes,
            hardware_count, hardware_ms, compute_count, compute_ms);
        for (unsigned j = 0; j < GS_GPU_HW_REASON_COUNT; j++) fprintf(p->trace, ",%u,%.6f", reason_count[j], reason_ms[j]);
        for (unsigned j = 0; j < 8; j++) fprintf(p->trace, ",%u,%.6f", kind_count[j], kind_ms[j]);
        for (unsigned j = 0; j < 5; j++) fprintf(p->trace, ",%u,%.6f", texture_count[j], texture_ms[j]);
        fputc('\n', p->trace);
        r->pending = 0;
    }
    fflush(p->trace);
    f = &p->trace_frame[p->trace_serial % TRACE_RING];
    if (f->pending) return; /* ring full: skip interval, never block */
    f->pending = 1; f->frame = p->trace_serial; f->count = f->overflow = f->scanout = 0; f->cpu_begin = ticks();
    ID3D11DeviceContext_Begin(p->context, (ID3D11Asynchronous *)f->disjoint);
    ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)f->stamp[0]);
}

void gs_gpu_profile(Gs *g, double seconds)
{
    Gpu *gpu = g->backend;
    LARGE_INTEGER frequency;
    if (!gpu || !gpu->profile) return;
    if (gpu->raster_stats.hardware_enabled) {
        const GsGpuRasterStats *s = &gpu->raster_stats;
        fprintf(stderr, "profile GS hardware: enabled=%d available=%d batches=%llu primitives=%llu reference=%llu rejects=%llu,%llu,%llu,%llu,%llu,%llu,%llu\n",
            s->hardware_enabled, s->hardware_available, s->hardware_batches, s->hardware_primitives, s->reference_batches,
            s->rejected[0],s->rejected[1],s->rejected[2],s->rejected[3],s->rejected[4],s->rejected[5],s->rejected[6]);
    }
    QueryPerformanceFrequency(&frequency);
    double ms = 1000.0 / (double)frequency.QuadPart / seconds;
    fprintf(stderr, "profile GS: %.1f draws/s, %.1f snapshots/s, %.1f uploads/s, draw %.1f ms/s, readback %.1f ms/s, %.1f dispatches/s, %.1f local copies/s\n",
            gpu->draws / seconds, gpu->snapshots / seconds, gpu->uploads / seconds,
            gpu->draw_ticks * ms, gpu->sync_ticks * ms, gpu->dispatches / seconds, gpu->copies / seconds);
    if (gpu->native) native_profile(gpu->native);
    gpu->draw_ticks = gpu->sync_ticks = gpu->draws = gpu->snapshots = gpu->uploads = 0;
    gpu->dispatches = gpu->copies = 0;
}

static void sync_vram(Gs *g)
{
    Gpu *gpu = g->backend;
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (!gpu || !gpu->dirty) return;
    uint64_t start = gpu->profile ? ticks() : 0;
    flush(gpu);
    ID3D11DeviceContext_CopyResource(gpu->context, (ID3D11Resource *)gpu->readback, (ID3D11Resource *)gpu->vram);
    HRESULT hr = ID3D11DeviceContext_Map(gpu->context, (ID3D11Resource *)gpu->readback, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) { fprintf(stderr, "gs: GPU readback failed (%08lX)\n", hr); exit(1); }
    /* Unuploaded CPU changes on other pages must survive this readback. */
    for (unsigned page = 0; page < 512; page++)
        if (gpu->cpu_dirty[page / 32] & (1u << (page & 31)))
            memcpy((uint8_t *)g->vram + page * 8192, (uint8_t *)mapped.pData + page * 8192, 8192);
    ID3D11DeviceContext_Unmap(gpu->context, (ID3D11Resource *)gpu->readback, 0);
    gpu->dirty = 0;
    memset(gpu->cpu_dirty, 0, sizeof gpu->cpu_dirty);
    if (gpu->profile) gpu->sync_ticks += ticks() - start;
}

/* Reads back only the GPU-written pages in the range. The game reads small rectangles every
   frame (sceGsExecStoreImage from func_00228328); a whole 4 MB readback for each took a third of
   a race frame. Other pages stay marked until something needs them. */
static void sync_range(Gs *g, unsigned address, unsigned bytes)
{
    Gpu *gpu = g->backend;
    uint32_t want[16] = {0};
    int any = 0, left = 0;
    if (!gpu || !gpu->dirty || !bytes) return;
    for (unsigned p = address / 8192; p <= (address + bytes - 1) / 8192 && p < address / 8192 + 512; p++) {
        unsigned page = p & 511;
        if (gpu->cpu_dirty[page / 32] & (1u << (page & 31))) want[page / 32] |= 1u << (page & 31), any = 1;
    }
    if (!any) return;
    uint64_t start = gpu->profile ? ticks() : 0;
    flush(gpu);
    for (unsigned page = 0; page < 512;) {
        if (!(want[page / 32] & (1u << (page & 31)))) { page++; continue; }
        unsigned end = page;
        while (end < 512 && want[end / 32] & (1u << (end & 31))) end++;
        D3D11_BOX box = {page * 8192, 0, 0, end * 8192, 1, 1};
        ID3D11DeviceContext_CopySubresourceRegion(gpu->context, (ID3D11Resource *)gpu->readback, 0, page * 8192, 0, 0,
                                                  (ID3D11Resource *)gpu->vram, 0, &box);
        page = end;
    }
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = ID3D11DeviceContext_Map(gpu->context, (ID3D11Resource *)gpu->readback, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) { fprintf(stderr, "gs: GPU readback failed (%08lX)\n", hr); exit(1); }
    for (unsigned page = 0; page < 512; page++)
        if (want[page / 32] & (1u << (page & 31)))
            memcpy((uint8_t *)g->vram + page * 8192, (uint8_t *)mapped.pData + page * 8192, 8192);
    ID3D11DeviceContext_Unmap(gpu->context, (ID3D11Resource *)gpu->readback, 0);
    for (unsigned i = 0; i < 16; i++) left |= (gpu->cpu_dirty[i] &= ~want[i]) != 0;
    gpu->dirty = left;
    if (gpu->profile) gpu->sync_ticks += ticks() - start;
}

/* Deferred reads (gs_defer_read_image): copy the GPU-written pages among want into the staging
   buffer, in order with the draws, and submit; fetch_pages maps it when the data is needed. */
static void stage_pages(Gs *g, const uint32_t want[16], uint32_t staged[16])
{
    Gpu *gpu = g->backend;
    int any = 0;
    for (unsigned i = 0; i < 16; i++) any |= (staged[i] = gpu->dirty ? want[i] & gpu->cpu_dirty[i] : 0) != 0;
    if (!any) return;
    flush(gpu);
    for (unsigned page = 0; page < 512;) {
        if (!(staged[page / 32] & (1u << (page & 31)))) { page++; continue; }
        unsigned end = page;
        while (end < 512 && staged[end / 32] & (1u << (end & 31))) end++;
        D3D11_BOX box = {page * 8192, 0, 0, end * 8192, 1, 1};
        ID3D11DeviceContext_CopySubresourceRegion(gpu->context, (ID3D11Resource *)gpu->staged, 0, page * 8192, 0, 0,
                                                  (ID3D11Resource *)gpu->vram, 0, &box);
        page = end;
    }
    ID3D11DeviceContext_Flush(gpu->context);
}

static void fetch_pages(Gs *g, const uint32_t staged[16], uint32_t *image)
{
    Gpu *gpu = g->backend;
    D3D11_MAPPED_SUBRESOURCE mapped;
    uint64_t start = gpu->profile ? ticks() : 0;
    HRESULT hr = ID3D11DeviceContext_Map(gpu->context, (ID3D11Resource *)gpu->staged, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) { fprintf(stderr, "gs: GPU readback failed (%08lX)\n", hr); exit(1); }
    for (unsigned page = 0; page < 512; page++)
        if (staged[page / 32] & (1u << (page & 31)))
            memcpy((uint8_t *)image + page * 8192, (uint8_t *)mapped.pData + page * 8192, 8192);
    ID3D11DeviceContext_Unmap(gpu->context, (ID3D11Resource *)gpu->staged, 0);
    if (gpu->profile) gpu->sync_ticks += ticks() - start;
}

static void upload_pages(Gs *g)
{
    Gpu *gpu = g->backend;
    if (gpu->uploaded == g->pixels_written) return;
    flush(gpu);
    unsigned any = 0;
    for (unsigned i = 0; i < 16; i++) any |= g->host_dirty[i];
    /* Initial contents and explicit whole-image writes have no page range. */
    if (gpu->uploaded == UINT64_MAX || !any) memset(g->host_dirty, 255, sizeof g->host_dirty);
    for (unsigned page = 0; page < 512;) {
        if (!(g->host_dirty[page / 32] & (1u << (page & 31)))) { page++; continue; }
        unsigned first = page++;
        while (page < 512 && (g->host_dirty[page / 32] & (1u << (page & 31)))) page++;
        if (gpu->trace) gpu->trace_upload_bytes += (page - first) * 8192;
        D3D11_BOX box = {first * 8192, 0, 0, page * 8192, 1, 1};
        ID3D11DeviceContext_UpdateSubresource(gpu->context, (ID3D11Resource *)gpu->vram,
                                             0, &box, (uint8_t *)g->vram + first * 8192, 0, 0);
    }
    for (unsigned i = 0; i < 16; i++) gpu->texture_dirty[i] |= g->host_dirty[i];
    memset(g->host_dirty, 0, sizeof g->host_dirty);
    gpu->uploaded = g->pixels_written;
    gpu->uploads++;
}

#ifdef DHD_GS_TEST_BUILD
int gs_gpu_readback_all(Gs *g, uint32_t *image)
{
    Gpu *gpu = g ? g->backend : NULL;
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (!gpu || !image) return 0;
    flush(gpu);
    upload_pages(g);
    ID3D11DeviceContext_CopyResource(gpu->context, (ID3D11Resource *)gpu->readback,
                                    (ID3D11Resource *)gpu->vram);
    HRESULT hr = ID3D11DeviceContext_Map(gpu->context, (ID3D11Resource *)gpu->readback,
                                        0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) return 0;
    memcpy(image, mapped.pData, GS_VRAM_SIZE);
    ID3D11DeviceContext_Unmap(gpu->context, (ID3D11Resource *)gpu->readback, 0);
    return 1;
}
#endif

static void texture_pages(Gpu *gpu, Gs *g, unsigned ctxt)
{
    uint64_t tex = g->reg[GS_TEX0_1 + ctxt], clamp = g->reg[GS_CLAMP_1 + ctxt];
    uint64_t tex1 = g->reg[GS_TEX1_1 + ctxt];
    uint64_t mip[2] = {g->reg[GS_MIPTBP1_1 + ctxt], g->reg[GS_MIPTBP2_1 + ctxt]};
    if (gpu->texture_valid && gpu->texture_key == tex && gpu->clamp_key == clamp &&
        gpu->tex1_key == tex1 && gpu->mip_key[0] == mip[0] && gpu->mip_key[1] == mip[1]) return;
    unsigned mmin = (unsigned)(tex1 >> 6) & 7, levels = (unsigned)(tex1 >> 2) & 7;
    if (levels > 6) levels = 6;
    if (mmin < 2 || mmin > 5) levels = 0;
    memset(gpu->texture_pages, 0, sizeof gpu->texture_pages);
    for (unsigned level = 0; level <= levels; level++) {
        unsigned lo[2] = {0,0}, hi[2];
        for (unsigned axis = 0; axis < 2; axis++) {
            unsigned mode = (unsigned)(clamp >> (axis * 2)) & 3;
            unsigned minimum = ((unsigned)(clamp >> (4 + axis * 20)) & 1023) >> level;
            unsigned maximum = ((unsigned)(clamp >> (14 + axis * 20)) & 1023) >> level;
            unsigned size = (unsigned)(tex >> (26 + axis * 4)) & 15;
            size = size > level ? size - level : 0;
            hi[axis] = size >= 11 ? 2047 : (1u << size) - 1;
            if (mode == 2) { lo[axis] = minimum; hi[axis] = maximum; }
            if (mode == 3) { lo[axis] = maximum; hi[axis] = maximum | minimum; }
            if (lo[axis] > hi[axis]) { lo[axis] = 0; hi[axis] = 2047; }
        }
        uint64_t location = level ? mip[(level - 1) / 3] >> (((level - 1) % 3) * 20) : tex;
        gs_mark_pages(gpu->texture_pages, (unsigned)(tex >> 20) & 63, (unsigned)location & 16383,
                      (unsigned)(location >> 14) & 63, lo[0], lo[1], hi[0], hi[1]);
    }
    gpu->texture_key = tex; gpu->clamp_key = clamp; gpu->texture_valid = 1;
    gpu->tex1_key = tex1; gpu->mip_key[0] = mip[0]; gpu->mip_key[1] = mip[1];
}

static int minimum(int a, int b) { return a < b ? a : b; }
static int maximum(int a, int b) { return a > b ? a : b; }

static int independent_pixels(Gs *g, unsigned ctxt)
{
    uint64_t frame = g->reg[GS_FRAME_1 + ctxt], z = g->reg[GS_ZBUF_1 + ctxt];
    uint64_t scissor = g->reg[GS_SCISSOR_1 + ctxt];
    unsigned bw = (unsigned)(frame >> 16) & 63;
    unsigned x = (unsigned)(scissor >> 16) & 2047, y = (unsigned)(scissor >> 48) & 2047;
    /* Aliased rows or overlapping color/Z surfaces can communicate across
       pixels. Keep a dispatch boundary between such primitives. */
    if (!bw || x >= bw * 64 || (y / 32 + 1) * bw > 512) return 0;
    uint64_t test = g->reg[GS_TEST_1 + ctxt];
    if ((z >> 32 & 1) && (!(test >> 16 & 1) || (test >> 17 & 3) <= 1)) return 1;
    uint32_t color[16] = {0}, depth[16] = {0};
    gs_mark_pages(color, (unsigned)(frame >> 24) & 63, ((unsigned)frame & 511)*32, bw, 0,0,x,y);
    gs_mark_pages(depth, 48 | ((unsigned)(z >> 24) & 15), ((unsigned)z & 511)*32, bw, 0,0,x,y);
    for (unsigned i = 0; i < 16; i++) if (color[i] & depth[i]) return 0;
    return 1;
}

/* Each 8x8 tile receives primitive indices in submission order. Pixels execute
   that list serially, retaining GS blending/Z order without a dispatch per
   triangle. A state change or texture-feedback hazard ends the batch. */
static void flush(Gpu *gpu)
{
    if (!gpu->pending_count) return;
    unsigned rejection = hardware_rejection(gpu);
    unsigned x0 = 2048, y0 = 2048, x1 = 0, y1 = 0;
    for (unsigned i = 0; i < gpu->pending_count; i++) {
        const uint32_t *b = gpu->pending[i].bounds;
        x0 = minimum(x0, b[0] / 8); y0 = minimum(y0, b[1] / 8);
        x1 = maximum(x1, (b[0] + b[2] - 1) / 8); y1 = maximum(y1, (b[1] + b[3] - 1) / 8);
    }
    unsigned width = x1 - x0 + 1, height = y1 - y0 + 1, tiles = width * height;
    uint32_t *bins = gpu->bin_data;
    {
        memset(bins, 0, tiles * 8);
        for (unsigned pass = 0; pass < 2; pass++) {
            for (unsigned i = 0; i < gpu->pending_count; i++) {
                const uint32_t *b = gpu->pending[i].bounds;
                unsigned right = (b[0] + b[2] - 1) / 8, bottom = (b[1] + b[3] - 1) / 8;
                for (unsigned y = b[1] / 8; y <= bottom; y++)
                    for (unsigned x = b[0] / 8; x <= right; x++) {
                        unsigned tile = (y - y0) * width + x - x0;
                        if (pass) bins[bins[tile * 2] + bins[tile * 2 + 1]] = i;
                        bins[tile * 2 + 1]++;
                    }
            }
            if (!pass) {
                unsigned offset = tiles * 2;
                for (unsigned i = 0; i < tiles; i++) {
                    bins[i * 2] = offset; offset += bins[i * 2 + 1]; bins[i * 2 + 1] = 0;
                }
            }
        }
    }
    Constants *c = &gpu->state;
    c->bounds[0] = x0 * 8; c->bounds[1] = y0 * 8;
    c->bounds[2] = width * 8; c->bounds[3] = height * 8;
    c->info[0] = width;
    c->info[1] = gpu->batch_safe ? 1u : 0u; /* independent pixels; packed pixels can share words */
    D3D11_BOX box = {0,0,0,gpu->pending_count * sizeof(Primitive),1,1};
    ID3D11DeviceContext_UpdateSubresource(gpu->context, (ID3D11Resource *)gpu->primitives, 0, &box, gpu->pending, 0, 0);
    box.right = (tiles * 2 + gpu->memberships) * 4;
    ID3D11DeviceContext_UpdateSubresource(gpu->context, (ID3D11Resource *)gpu->bins, 0, &box, bins, 0, 0);
    ID3D11DeviceContext_UpdateSubresource(gpu->context, (ID3D11Resource *)gpu->constants, 0, NULL, c, 0, 0);
    ID3D11ShaderResourceView *views[] = {gpu->srv, gpu->primitive_srv, gpu->bin_srv};
    if (rejection == GS_GPU_HW_REASON_COUNT) {
        hardware_submit(gpu, views);
        gpu->pending_count = gpu->memberships = 0;
        gpu->dispatches++;
        return;
    }
    gpu->raster_stats.reference_batches++;
    gpu->raster_stats.reference_primitives += gpu->pending_count;
    gpu->raster_stats.rejected[rejection]++;
    ID3D11DeviceContext_CSSetShader(gpu->context, gpu->shader, NULL, 0);
    ID3D11DeviceContext_CSSetConstantBuffers(gpu->context, 0, 1, &gpu->constants);
    ID3D11DeviceContext_CSSetShaderResources(gpu->context, 0, 3, views);
    ID3D11DeviceContext_CSSetUnorderedAccessViews(gpu->context, 0, 1, &gpu->uav, NULL);
    gpu->trace_dispatch_hardware = 0;
    gpu->trace_dispatch_rejection = rejection;
    trace_dispatch(gpu, 0);
    ID3D11DeviceContext_Dispatch(gpu->context, width, height, 1);
    trace_dispatch(gpu, 1);
    if (gpu->trace) { gpu->trace_primitives += gpu->pending_count; gpu->trace_raster_count++; }
    ID3D11UnorderedAccessView *none = NULL;
    ID3D11ShaderResourceView *no_views[3] = {NULL,NULL,NULL};
    ID3D11DeviceContext_CSSetUnorderedAccessViews(gpu->context, 0, 1, &none, NULL);
    ID3D11DeviceContext_CSSetShaderResources(gpu->context, 0, 3, no_views);
    gpu->pending_count = gpu->memberships = 0;
    gpu->dispatches++;
}

/* Local diagnostics include game textures: keep the requested directory under build/. */
static void trace_draw(Gs *g, unsigned prim, const GsVertex *v, unsigned count)
{
    static FILE *log;
    static int initialized, fullscreen, fogged, large;
    static double after;
    static unsigned draws, seen_count, skip, limit = 2048;
    static uint64_t seen[64];
    static const char *dir;
    char path[1024];
    if (!initialized) {
        initialized = 1;
        dir = getenv("DHD_GS_TRACE");
        fullscreen = getenv("DHD_GS_TRACE_FULLSCREEN") != NULL;
        fogged = getenv("DHD_GS_TRACE_FOGGED") != NULL;
        large = getenv("DHD_GS_TRACE_LARGE") != NULL;
        const char *at = getenv("DHD_GS_TRACE_AT"); after = at ? atof(at) : 0;
        const char *after = getenv("DHD_GS_TRACE_SKIP");
        skip = after ? strtoul(after, NULL, 0) : 0;
        const char *count = getenv("DHD_GS_TRACE_COUNT");
        if (count) limit = strtoul(count, NULL, 0);
        if (dir) {
            snprintf(path, sizeof path, "%s/draws.txt", dir);
            log = fopen(path, "w");
        }
    }
    if (!dir || draws >= limit) return;
    if (g->trace_seconds < after) return;
    if (large) {
        if (count != 3) return;
        unsigned xmin = 65535, ymin = 65535, xmax = 0, ymax = 0;
        for (unsigned j = 0; j < count; j++) {
            if (v[j].x < xmin) xmin = v[j].x;
            if (v[j].x > xmax) xmax = v[j].x;
            if (v[j].y < ymin) ymin = v[j].y;
            if (v[j].y > ymax) ymax = v[j].y;
        }
        if (xmax-xmin < 5120 && ymax-ymin < 5120) return;
    }
    if (fogged && (!(prim & 32) || count != 3 || v[0].fog || v[1].fog || v[2].fog)) return;
    g->suspect_draws++;
    if (skip) { skip--; return; }
    if (!log) return;
    if (fullscreen) {
        unsigned left = 65535, right = 0;
        for (unsigned j = 0; j < count; j++) {
            if (v[j].x < left) left = v[j].x;
            if (v[j].x > right) right = v[j].x;
        }
        if ((prim & 23) != 6 || right - left < 5000) return;
    }
    unsigned ctxt = prim >> 9 & 1;
    uint64_t tex = g->reg[GS_TEX0_1 + ctxt];
    unsigned i;
    for (i = 0; i < seen_count && seen[i] != tex; i++) {}
    if ((prim & 16) && i == seen_count && seen_count < 64) {
        seen[seen_count++] = tex;
        sync_vram(g);
        snprintf(path, sizeof path, "%s/texture-%016llX.bin", dir, tex);
        FILE *f = fopen(path, "wb");
        if (f) { fwrite(g->vram, GS_VRAM_SIZE, 1, f); fclose(f); }
    }
    fprintf(log, "%u prim=%X tex=%016llX tex1=%llX clamp=%llX offset=%llX\n",
            draws++, prim, tex, g->reg[GS_TEX1_1 + ctxt], g->reg[GS_CLAMP_1 + ctxt], g->reg[GS_XYOFFSET_1 + ctxt]);
    fprintf(log, "  time=%.6f path=%u entry=%04X top=%X itop=%X pc=%04X clip=%06X kick=%X adc=%llu fogcol=%llX mip=%llX,%llX\n",
            g->trace_seconds, g->source_path, g->vu_entry, g->vu_top, g->vu_itop,
            g->vu_pc, g->vu_clip, g->vu_qword, g->adc_skips, g->reg[GS_FOGCOL],
            g->reg[GS_MIPTBP1_1 + ctxt], g->reg[GS_MIPTBP2_1 + ctxt]);
    fprintf(log, "  frame=%llX zbuf=%llX test=%llX alpha=%llX scissor=%llX\n",
            g->reg[GS_FRAME_1 + ctxt], g->reg[GS_ZBUF_1 + ctxt], g->reg[GS_TEST_1 + ctxt],
            g->reg[GS_ALPHA_1 + ctxt], g->reg[GS_SCISSOR_1 + ctxt]);
    fprintf(log, "  overlay=%u program=%04X\n", g->vu_overlay, g->vu_program);
    unsigned ox = (unsigned)g->reg[GS_XYOFFSET_1 + ctxt] & 65535;
    unsigned oy = (unsigned)(g->reg[GS_XYOFFSET_1 + ctxt] >> 32) & 65535;
    for (i = 0; i < count; i++)
        fprintf(log, "  xy=%u,%u window=%d,%d z=%08X uv=%u,%u stq=%.9g,%.9g,%.9g rgba=%08X fog=%u\n",
                v[i].x, v[i].y, (int)v[i].x-(int)ox, (int)v[i].y-(int)oy, v[i].z,
                v[i].u, v[i].v, v[i].s, v[i].t, v[i].q, v[i].rgba, v[i].fog);
    fflush(log);
}

static void draw(Gs *g, unsigned type, const GsVertex *v, unsigned count)
{
    /* Diagnostic isolation by VU1 byte entry. Execute VU/GIF and retain all
       register/vertex state; suppress only this backend draw, never game math. */
    static int skip_initialized;
    static unsigned skip_entries[64], skip_count, skip_overlay, skip_programs[64], program_count;
    if (!skip_initialized) {
        const char *p = getenv("DHD_DRAW_SKIP_VU");
        const char *overlay = getenv("DHD_DRAW_SKIP_OVERLAY");
        if (overlay) skip_overlay = !strcmp(overlay, "A") ? 1 : !strcmp(overlay, "B") ? 2 : 0;
        skip_initialized = 1;
        while (p && *p && skip_count < 64) {
            char *end;
            unsigned entry = strtoul(p, &end, 16);
            if (end == p || entry >= 0x4000) break;
            skip_entries[skip_count++] = entry;
            p = *end == ',' ? end + 1 : end;
        }
        p = getenv("DHD_DRAW_SKIP_PROGRAM");
        while (p && *p && program_count < 64) {
            char *end;
            unsigned entry = strtoul(p, &end, 16);
            if (end == p || entry >= 0x4000) break;
            skip_programs[program_count++] = entry;
            p = *end == ',' ? end + 1 : end;
        }
    }
    if (g->source_path == 1 && skip_overlay && g->vu_overlay == skip_overlay) return;
    if (g->source_path == 1)
        for (unsigned i = 0; i < program_count; i++)
            if (g->vu_program == skip_programs[i]) return;
    if (g->source_path == 1)
        for (unsigned i = 0; i < skip_count; i++)
            if (g->vu_entry == skip_entries[i]) return;
    Gpu *gpu = g->backend;
    uint64_t start = gpu->profile ? ticks() : 0;
    Primitive c = {0};
    unsigned prim = (unsigned)((g->reg[GS_PRIM] & 7) |
                    ((g->reg[GS_PRMODECONT] & 1 ? g->reg[GS_PRIM] : g->reg[GS_PRMODE]) & 0x7F8));
    unsigned ctxt = prim >> 9 & 1;
    trace_draw(g, prim, v, count);
    /* Keep integer Z exact through plane setup and interpolation. GS software
       rendering uses double precision for depth, independently of ST/RGBA. */
    double depth[4] = {v[0].z, 0, 0, 0};
    if (type == 1 || type == 2) {
        int dx = (int)v[1].x - (int)v[0].x, dy = (int)v[1].y - (int)v[0].y;
        unsigned axis = abs(dx) >= abs(dy) ? 1 : 2;
        int distance = axis == 1 ? dx : dy;
        if (distance) depth[axis] = ((double)v[1].z - v[0].z) / distance;
    } else if (count == 3) {
        double dx1 = (int)v[1].x - (int)v[0].x, dy1 = (int)v[1].y - (int)v[0].y;
        double dx2 = (int)v[2].x - (int)v[0].x, dy2 = (int)v[2].y - (int)v[0].y;
        double dz1 = (double)v[1].z - v[0].z, dz2 = (double)v[2].z - v[0].z;
        double area = dx1 * dy2 - dy1 * dx2;
        if (area) { depth[1] = (dz1 * dy2 - dy1 * dz2) / area; depth[2] = (dx1 * dz2 - dz1 * dx2) / area; }
    }
    memcpy(c.depth, depth, sizeof depth);
    uint64_t offset = g->reg[GS_XYOFFSET_1 + ctxt], scissor = g->reg[GS_SCISSOR_1 + ctxt];
    int minx = 65536, miny = 65536, maxx = -65536, maxy = -65536;
    for (unsigned i = 0; i < count; i++) {
        int x = (int)v[i].x - (int)(offset & 65535), y = (int)v[i].y - (int)(offset >> 32 & 65535);
        minx = minimum(minx, x); miny = minimum(miny, y); maxx = maximum(maxx, x); maxy = maximum(maxy, y);
        c.position[i][0] = (uint32_t)x; c.position[i][1] = (uint32_t)y;
        c.position[i][2] = v[i].z; c.position[i][3] = v[i].rgba;
        c.uv[i][0] = v[i].u; c.uv[i][1] = v[i].v; c.uv[i][2] = v[i].fog;
        c.stq[i][0] = v[i].s; c.stq[i][1] = v[i].t; c.stq[i][2] = v[i].q;
    }
    minx = maximum(minx >> 4, (int)(scissor & 2047));
    miny = maximum(miny >> 4, (int)(scissor >> 32 & 2047));
    maxx = minimum((maxx + 15) >> 4, (int)(scissor >> 16 & 2047));
    maxy = minimum((maxy + 15) >> 4, (int)(scissor >> 48 & 2047));
    if (gpu->clip_on) {
        minx = maximum(minx, gpu->clip[0]); miny = maximum(miny, gpu->clip[1]);
        maxx = minimum(maxx, gpu->clip[2]); maxy = minimum(maxy, gpu->clip[3]);
    }
    if (minx > maxx || miny > maxy) return;
    if (gpu->state_version != g->state_version || gpu->state_context != ctxt) {
        flush(gpu);
        static const unsigned registers[] = {6,7,8,9,20,21,34,52,53,54,55,59,61,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79};
        for (unsigned r = 0; r < sizeof registers / sizeof *registers; r++) {
            unsigned i = registers[r];
            gpu->state.reg[i][0] = (uint32_t)g->reg[i]; gpu->state.reg[i][1] = (uint32_t)(g->reg[i] >> 32);
        }
        for (unsigned i = 0; i < 256; i++) gpu->state.palette[i] = gs_palette(g, g->reg[GS_TEX0_1 + ctxt], i);
        gpu->state_version = g->state_version; gpu->state_context = ctxt;
        gpu->batch_safe = independent_pixels(g, ctxt);
        gpu->hardware_rejection = hardware_state_rejection(g, ctxt);
    }
    c.bounds[0] = (uint32_t)minx; c.bounds[1] = (uint32_t)miny;
    c.bounds[2] = (uint32_t)(maxx - minx + 1); c.bounds[3] = (uint32_t)(maxy - miny + 1);
    c.info[0] = type; c.info[1] = prim; c.info[2] = count;
    unsigned memberships = ((unsigned)maxx / 8 - (unsigned)minx / 8 + 1) *
                           ((unsigned)maxy / 8 - (unsigned)miny / 8 + 1);
    if (gpu->pending_count == gpu->batch_limit || gpu->memberships + memberships + 2 * 65536 > MAX_BIN_WORDS)
        flush(gpu);
    upload_pages(g);
    if (prim & 16) {
        texture_pages(gpu, g, ctxt);
        for (unsigned i = 0; i < 16; i++) if (gpu->texture_pages[i] & gpu->texture_dirty[i]) {
            flush(gpu);
            ID3D11DeviceContext_CopyResource(gpu->context, (ID3D11Resource *)gpu->snapshot, (ID3D11Resource *)gpu->vram);
            gpu->snapshots++;
            memset(gpu->texture_dirty, 0, sizeof gpu->texture_dirty);
            break;
        }
    }
    Primitive *p = &gpu->pending[gpu->pending_count++];
    memcpy(p->position, c.position, sizeof p->position); memcpy(p->uv, c.uv, sizeof p->uv);
    memcpy(p->stq, c.stq, sizeof p->stq); memcpy(p->info, c.info, sizeof p->info);
    memcpy(p->bounds, c.bounds, sizeof p->bounds);
    memcpy(p->depth, c.depth, sizeof p->depth);
    gpu->memberships += memberships;
    gpu->dirty = 1;
    uint32_t written[16] = {0};
    uint64_t frame = g->reg[GS_FRAME_1 + ctxt], zbuf = g->reg[GS_ZBUF_1 + ctxt];
    unsigned bw = (unsigned)(frame >> 16) & 63;
    gs_mark_pages(written, (unsigned)(frame >> 24) & 63, ((unsigned)frame & 511) * 32, bw, minx,miny,maxx,maxy);
    if (!(zbuf >> 32 & 1))
        gs_mark_pages(written, 48 | ((unsigned)(zbuf >> 24) & 15), ((unsigned)zbuf & 511) * 32, bw, minx,miny,maxx,maxy);
    for (unsigned i = 0; i < 16; i++) { gpu->cpu_dirty[i] |= written[i]; gpu->texture_dirty[i] |= written[i]; }
    gpu->draws++;
    if (!gpu->batch_safe) flush(gpu);
    if (gpu->profile) gpu->draw_ticks += ticks() - start;
}

/* TRXDIR 2 on the GPU. The CPU path read all of VRAM back first; the race does several of these
   a frame (a third of its time). Overlapping or wrapping rectangles keep the CPU's pixel order. */
static int local_copy(Gs *g)
{
    Gpu *gpu = g->backend;
    uint64_t b = g->reg[GS_BITBLTBUF], p = g->reg[GS_TRXPOS], r = g->reg[GS_TRXREG];
    uint32_t c[12] = {0}, from[16] = {0}, to[16] = {0};
    unsigned width = (unsigned)r & 4095, height = (unsigned)(r >> 32) & 4095;
    c[0] = (unsigned)(b >> 24) & 63; c[1] = (unsigned)b & 0x3FFF; c[2] = (unsigned)(b >> 16) & 63;
    c[3] = (unsigned)p & 2047; c[4] = (unsigned)(p >> 16) & 2047;
    c[5] = (unsigned)(b >> 56) & 63; c[6] = (unsigned)(b >> 32) & 0x3FFF; c[7] = (unsigned)(b >> 48) & 63;
    c[8] = width; c[9] = height; c[10] = (unsigned)(p >> 32) & 2047; c[11] = (unsigned)(p >> 48) & 2047;
    if (!gpu || !gpu->copy_shader || !width || !height || c[3] + width > 2048 || c[4] + height > 2048 ||
        c[10] + width > 2048 || c[11] + height > 2048)
        return 0;
    gs_mark_pages(from, c[0], c[1], c[2], c[3], c[4], c[3] + width - 1, c[4] + height - 1);
    gs_mark_pages(to, c[5], c[6], c[7], c[10], c[11], c[10] + width - 1, c[11] + height - 1);
    for (unsigned i = 0; i < 16; i++) if (from[i] & to[i]) return 0;
    upload_pages(g);
    flush(gpu);
    ID3D11DeviceContext_UpdateSubresource(gpu->context, (ID3D11Resource *)gpu->copy_constants, 0, NULL, c, 0, 0);
    ID3D11DeviceContext_CSSetShader(gpu->context, gpu->copy_shader, NULL, 0);
    ID3D11DeviceContext_CSSetConstantBuffers(gpu->context, 2, 1, &gpu->copy_constants);
    ID3D11DeviceContext_CSSetUnorderedAccessViews(gpu->context, 0, 1, &gpu->uav, NULL);
    ID3D11DeviceContext_Dispatch(gpu->context, (width + 7) / 8, (height + 7) / 8, 1);
    ID3D11UnorderedAccessView *none = NULL;
    ID3D11DeviceContext_CSSetUnorderedAccessViews(gpu->context, 0, 1, &none, NULL);
    for (unsigned i = 0; i < 16; i++) { gpu->cpu_dirty[i] |= to[i]; gpu->texture_dirty[i] |= to[i]; }
    gpu->dirty = 1;
    gpu->copies++;
    return 1;
}

void gs_gpu_destroy(Gs *g)
{
    Gpu *p = g->backend;
    if (!p) return;
    sync_vram(g);
    if (p->trace) {
        for (unsigned i = 0; i < TRACE_RING; i++) {
            GpuTraceFrame *f = &p->trace_frame[i];
            if (f->disjoint) ID3D11Query_Release(f->disjoint);
            for (unsigned j = 0; j < 4; j++) if (f->stamp[j]) ID3D11Query_Release(f->stamp[j]);
            for (unsigned j = 0; j < TRACE_DISPATCHES * 2; j++) if (f->dispatch[j]) ID3D11Query_Release(f->dispatch[j]);
        }
        fclose(p->trace);
    }
#define RELEASE(member, type) if (p->member) type##_Release(p->member)
    RELEASE(hardware_vertex, ID3D11VertexShader); RELEASE(hardware_pixel, ID3D11PixelShader);
    RELEASE(hardware_rasterizer, ID3D11RasterizerState2); RELEASE(hardware_depth, ID3D11DepthStencilState);
    RELEASE(shader, ID3D11ComputeShader); RELEASE(srv, ID3D11ShaderResourceView);
    RELEASE(uav, ID3D11UnorderedAccessView); RELEASE(vram, ID3D11Buffer);
    RELEASE(snapshot, ID3D11Buffer); RELEASE(readback, ID3D11Buffer); RELEASE(constants, ID3D11Buffer);
    RELEASE(staged, ID3D11Buffer);
    RELEASE(primitives, ID3D11Buffer); RELEASE(bins, ID3D11Buffer);
    RELEASE(primitive_srv, ID3D11ShaderResourceView); RELEASE(bin_srv, ID3D11ShaderResourceView);
    RELEASE(display_shader, ID3D11ComputeShader); RELEASE(display_constants, ID3D11Buffer);
    RELEASE(copy_shader, ID3D11ComputeShader); RELEASE(copy_constants, ID3D11Buffer);
    RELEASE(vram_srv, ID3D11ShaderResourceView); RELEASE(display, ID3D11Texture2D);
    RELEASE(display_readback, ID3D11Texture2D); RELEASE(display_uav, ID3D11UnorderedAccessView);
    RELEASE(context, ID3D11DeviceContext); RELEASE(device, ID3D11Device);
#undef RELEASE
    free(p->bin_data); free(p); g->backend = NULL; g->draw = NULL; g->sync = NULL; g->sync_range = NULL;
    g->local_copy = NULL; g->stage_pages = NULL; g->fetch_pages = NULL;
    g->flush_draw = NULL;
    g->write_high = NULL;
}

int gs_gpu_display(Gs *g, unsigned *width, unsigned *height)
{
    Gpu *p = g->backend;
    uint32_t c[12] = {0};
    uint64_t pmode = gs_priv_read(g, GS_PMODE);
    unsigned circuit = pmode & 1 ? 0 : 1;
    uint64_t fb = gs_priv_read(g, circuit ? GS_DISPFB2 : GS_DISPFB1);
    uint64_t display = gs_priv_read(g, circuit ? GS_DISPLAY2 : GS_DISPLAY1);
    c[0] = (unsigned)(fb >> 15) & 31; c[1] = ((unsigned)fb & 511) * 32;
    c[2] = (unsigned)(fb >> 9) & 63; c[3] = (unsigned)(fb >> 32) & 2047; c[4] = (unsigned)(fb >> 43) & 2047;
    c[5] = ((unsigned)(display >> 32) & 4095) + 1; c[5] /= ((unsigned)(display >> 23) & 15) + 1;
    c[6] = ((unsigned)(display >> 44) & 2047) + 1; c[6] /= ((unsigned)(display >> 27) & 3) + 1;
    c[7] = (gs_priv_read(g, GS_SMODE2) & 3) == 3;
    if (!p || !(pmode & 3) || !c[2] || !c[5] || !c[6] || c[5] > 1024 || c[6] > 1024) {
        return 0;
    }
    if (p->display_width != c[5] || p->display_height != c[6]) {
        if (p->display_uav) { ID3D11UnorderedAccessView_Release(p->display_uav); p->display_uav = NULL; }
        if (p->display) { ID3D11Texture2D_Release(p->display); p->display = NULL; }
        if (p->display_readback) { ID3D11Texture2D_Release(p->display_readback); p->display_readback = NULL; }
        p->display_width = p->display_height = 0;
        D3D11_TEXTURE2D_DESC d = {0};
        d.Width = c[5]; d.Height = c[6]; d.MipLevels = d.ArraySize = 1;
        d.Format = DXGI_FORMAT_R8G8B8A8_UNORM; d.SampleDesc.Count = 1;
        d.Usage = D3D11_USAGE_DEFAULT; d.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
        if (FAILED(ID3D11Device_CreateTexture2D(p->device, &d, NULL, &p->display))) return 0;
        if (FAILED(ID3D11Device_CreateUnorderedAccessView(p->device, (ID3D11Resource *)p->display, NULL, &p->display_uav))) return 0;
        d.Usage = D3D11_USAGE_STAGING; d.BindFlags = 0; d.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        if (FAILED(ID3D11Device_CreateTexture2D(p->device, &d, NULL, &p->display_readback))) return 0;
        p->display_width = c[5]; p->display_height = c[6];
    }
    flush(p);
    upload_pages(g);
    ID3D11DeviceContext_UpdateSubresource(p->context, (ID3D11Resource *)p->display_constants, 0, NULL, c, 0, 0);
    ID3D11DeviceContext_CSSetShader(p->context, p->display_shader, NULL, 0);
    ID3D11DeviceContext_CSSetConstantBuffers(p->context, 1, 1, &p->display_constants);
    ID3D11DeviceContext_CSSetShaderResources(p->context, 0, 1, &p->vram_srv);
    ID3D11DeviceContext_CSSetUnorderedAccessViews(p->context, 1, 1, &p->display_uav, NULL);
    if (p->trace && p->trace_frame[p->trace_serial % TRACE_RING].pending == 1)
        ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)p->trace_frame[p->trace_serial % TRACE_RING].stamp[1]);
    ID3D11DeviceContext_Dispatch(p->context, (c[5] + 7) / 8, (c[6] + 7) / 8, 1);
    ID3D11ShaderResourceView *no_srv = NULL;
    ID3D11UnorderedAccessView *no_uav = NULL;
    ID3D11DeviceContext_CSSetShaderResources(p->context, 0, 1, &no_srv);
    ID3D11DeviceContext_CSSetUnorderedAccessViews(p->context, 1, 1, &no_uav, NULL);
    if (p->trace && p->trace_frame[p->trace_serial % TRACE_RING].pending == 1)
        ID3D11DeviceContext_End(p->context, (ID3D11Asynchronous *)p->trace_frame[p->trace_serial % TRACE_RING].stamp[2]);
    if (p->trace) p->trace_frame[p->trace_serial % TRACE_RING].scanout = 1;
    *width = c[5]; *height = c[6];
    return 1;
}

void gs_gpu_copy_display(Gs *g, ID3D11Texture2D *target)
{
    Gpu *p = g->backend;
    ID3D11DeviceContext_CopyResource(p->context, (ID3D11Resource *)target, (ID3D11Resource *)p->display);
}

int gs_gpu_capture(Gs *g, uint32_t *pixels)
{
    Gpu *p = g->backend;
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (p && p->native) return native_capture(p->native, pixels);
    if (!p || !p->display_readback) return 0;
    ID3D11DeviceContext_CopyResource(p->context, (ID3D11Resource *)p->display_readback, (ID3D11Resource *)p->display);
    if (FAILED(ID3D11DeviceContext_Map(p->context, (ID3D11Resource *)p->display_readback, 0, D3D11_MAP_READ, 0, &mapped))) return 0;
    for (unsigned y = 0; y < p->display_height; y++)
        memcpy(pixels + y * p->display_width, (uint8_t *)mapped.pData + y * mapped.RowPitch, p->display_width * 4);
    ID3D11DeviceContext_Unmap(p->context, (ID3D11Resource *)p->display_readback, 0);
    return 1;
}

static HRESULT input_buffer(Gpu *gpu, unsigned bytes, unsigned stride,
                            ID3D11Buffer **buffer, ID3D11ShaderResourceView **view)
{
    D3D11_BUFFER_DESC b = {0};
    D3D11_SHADER_RESOURCE_VIEW_DESC s = {0};
    b.ByteWidth = bytes; b.Usage = D3D11_USAGE_DEFAULT; b.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    b.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; b.StructureByteStride = stride;
    HRESULT hr = ID3D11Device_CreateBuffer(gpu->device, &b, NULL, buffer);
    if (FAILED(hr)) return hr;
    s.Format = DXGI_FORMAT_UNKNOWN; s.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    s.Buffer.NumElements = bytes / stride;
    return ID3D11Device_CreateShaderResourceView(gpu->device, (ID3D11Resource *)*buffer, &s, view);
}

#include "gpu_native.inc"

static int exact_init(Gs *g, ID3D11Device *device, ID3D11DeviceContext *context)
{
    D3D11_FEATURE_DATA_DOUBLES doubles = {0};
    if (FAILED(ID3D11Device_CheckFeatureSupport(device, D3D11_FEATURE_DOUBLES, &doubles, sizeof doubles)) ||
        !doubles.DoublePrecisionFloatShaderOps) {
        fprintf(stderr, "gs: double-precision depth requires D3D11 double shader operations\n");
        return 0;
    }
    Gpu *p = calloc(1, sizeof *p);
    ID3DBlob *code = NULL, *errors = NULL;
    D3D11_BUFFER_DESC b = {0};
    D3D11_UNORDERED_ACCESS_VIEW_DESC u = {0};
    D3D11_SHADER_RESOURCE_VIEW_DESC s = {0};
    if (!p) return 0;
    p->device = device; p->context = context; p->uploaded = UINT64_MAX;
    p->state_version = UINT64_MAX;
    p->profile = getenv("DHD_PROFILE") != NULL;
    const char *trace_path = getenv("DHD_GPU_TRACE");
    if (trace_path) {
        p->trace = fopen(trace_path, "w");
        if (!p->trace) { perror(trace_path); abort(); }
        QueryPerformanceFrequency(&p->trace_frequency);
        fprintf(p->trace, "# qpc_frequency=%llu; detailed_interval=32; render_interval_includes_starvation\n", (uint64_t)p->trace_frequency.QuadPart);
        fprintf(p->trace, "frame,wall_end,raster_dispatches,cpu_begin,cpu_present_begin,cpu_end,disjoint,render_interval_ms,scanout_ms,copy_ms,raster_dispatch_ms,sampled_dispatches,overflow,primitives,vram_upload_bytes,hardware_draws,hardware_ms,compute_dispatches,compute_ms");
        for (unsigned j = 0; j < GS_GPU_HW_REASON_COUNT; j++) fprintf(p->trace, ",compute_reason_%u_count,compute_reason_%u_ms", j,j);
        for (unsigned j = 0; j < 8; j++) fprintf(p->trace, ",compute_primitive_%u_count,compute_primitive_%u_ms", j,j);
        for (unsigned j = 0; j < 5; j++) fprintf(p->trace, ",hardware_texture_%u_count,hardware_texture_%u_ms", j,j);
        fputc('\n', p->trace);
        for (unsigned i = 0; i < TRACE_RING; i++) {
            GpuTraceFrame *f = &p->trace_frame[i];
            f->disjoint = trace_query(p, D3D11_QUERY_TIMESTAMP_DISJOINT);
            if (!f->disjoint) abort();
            for (unsigned j = 0; j < 4; j++) if (!(f->stamp[j] = trace_query(p, D3D11_QUERY_TIMESTAMP))) abort();
        }
        for (unsigned j = 0; j < TRACE_DISPATCHES * 2; j++)
            if (!(p->trace_frame[0].dispatch[j] = trace_query(p, D3D11_QUERY_TIMESTAMP))) abort();
    }
    const char *limit = getenv("DHD_GS_BATCH");
    p->batch_limit = limit ? strtoul(limit, NULL, 0) : MAX_DRAWS;
    if (!p->batch_limit || p->batch_limit > MAX_DRAWS) p->batch_limit = MAX_DRAWS;
    p->bin_data = malloc(MAX_BIN_WORDS * sizeof *p->bin_data);
    ID3D11Device_AddRef(device); ID3D11DeviceContext_AddRef(context);
    g->backend = p;
    if (!p->bin_data) { gs_gpu_destroy(g); return 0; }
    const char *early_option = getenv("DHD_GS_EARLY_DEPTH");
    const char *address_option = getenv("DHD_GS_ADDRESS_SHIFT");
    const char *store_option = getenv("DHD_GS_EXCLUSIVE_WORD_STORE");
    D3D_SHADER_MACRO raster_defines[] = {
        {"DHD_EARLY_DEPTH", !early_option || atoi(early_option) ? "1" : "0"},
        {"DHD_ADDRESS_SHIFT", !address_option || atoi(address_option) ? "1" : "0"},
        {"DHD_EXCLUSIVE_WORD_STORE", !store_option || atoi(store_option) ? "1" : "0"},
        {NULL, NULL}
    };
    HRESULT hr = shader_compile(raster_defines, "main", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_IEEE_STRICTNESS, &code, &errors);
    if (errors) { fprintf(stderr, "%s", (const char *)ID3D10Blob_GetBufferPointer(errors)); ID3D10Blob_Release(errors); }
    if (FAILED(hr)) goto fail;
    hr = ID3D11Device_CreateComputeShader(device, ID3D10Blob_GetBufferPointer(code), ID3D10Blob_GetBufferSize(code), NULL, &p->shader);
    ID3D10Blob_Release(code);
    if (FAILED(hr)) goto fail;
    code = errors = NULL;
    hr = shader_compile(raster_defines, "scanout", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_IEEE_STRICTNESS, &code, &errors);
    if (errors) { fprintf(stderr, "%s", (const char *)ID3D10Blob_GetBufferPointer(errors)); ID3D10Blob_Release(errors); }
    if (FAILED(hr)) goto fail;
    hr = ID3D11Device_CreateComputeShader(device, ID3D10Blob_GetBufferPointer(code), ID3D10Blob_GetBufferSize(code), NULL, &p->display_shader);
    ID3D10Blob_Release(code);
    if (FAILED(hr)) goto fail;
    code = errors = NULL;
    hr = shader_compile(raster_defines, "local_copy", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_IEEE_STRICTNESS, &code, &errors);
    if (errors) { fprintf(stderr, "%s", (const char *)ID3D10Blob_GetBufferPointer(errors)); ID3D10Blob_Release(errors); }
    if (FAILED(hr)) goto fail;
    hr = ID3D11Device_CreateComputeShader(device, ID3D10Blob_GetBufferPointer(code), ID3D10Blob_GetBufferSize(code), NULL, &p->copy_shader);
    ID3D10Blob_Release(code);
    if (FAILED(hr)) goto fail;
    b.ByteWidth = GS_VRAM_SIZE; b.Usage = D3D11_USAGE_DEFAULT;
    b.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE; b.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
    if (FAILED(hr = ID3D11Device_CreateBuffer(device, &b, NULL, &p->vram))) goto fail;
    b.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    if (FAILED(hr = ID3D11Device_CreateBuffer(device, &b, NULL, &p->snapshot))) goto fail;
    b.BindFlags = b.MiscFlags = 0; b.Usage = D3D11_USAGE_STAGING; b.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    if (FAILED(hr = ID3D11Device_CreateBuffer(device, &b, NULL, &p->readback))) goto fail;
    if (FAILED(hr = ID3D11Device_CreateBuffer(device, &b, NULL, &p->staged))) goto fail;
    b.ByteWidth = sizeof(Constants); b.Usage = D3D11_USAGE_DEFAULT;
    b.CPUAccessFlags = 0; b.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    if (FAILED(hr = ID3D11Device_CreateBuffer(device, &b, NULL, &p->constants))) goto fail;
    b.ByteWidth = 48;
    if (FAILED(hr = ID3D11Device_CreateBuffer(device, &b, NULL, &p->display_constants))) goto fail;
    if (FAILED(hr = ID3D11Device_CreateBuffer(device, &b, NULL, &p->copy_constants))) goto fail;
    u.Format = DXGI_FORMAT_R32_TYPELESS; u.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    u.Buffer.NumElements = GS_VRAM_SIZE / 4; u.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
    if (FAILED(hr = ID3D11Device_CreateUnorderedAccessView(device, (ID3D11Resource *)p->vram, &u, &p->uav))) goto fail;
    s.Format = DXGI_FORMAT_R32_TYPELESS; s.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    s.BufferEx.NumElements = GS_VRAM_SIZE / 4; s.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
    if (FAILED(hr = ID3D11Device_CreateShaderResourceView(device, (ID3D11Resource *)p->snapshot, &s, &p->srv))) goto fail;
    if (FAILED(hr = ID3D11Device_CreateShaderResourceView(device, (ID3D11Resource *)p->vram, &s, &p->vram_srv))) goto fail;
    if (FAILED(hr = input_buffer(p, MAX_DRAWS * sizeof(Primitive), sizeof(Primitive), &p->primitives, &p->primitive_srv))) goto fail;
    if (FAILED(hr = input_buffer(p, MAX_BIN_WORDS * 4, 4, &p->bins, &p->bin_srv))) goto fail;
    hardware_init(p, raster_defines);
    g->draw = draw; g->sync = sync_vram; g->sync_range = sync_range;
    g->local_copy = local_copy;
    g->stage_pages = stage_pages; g->fetch_pages = fetch_pages;
    return 1;
fail:
    fprintf(stderr, "gs: GPU rasterizer initialization failed (%08lX)\n", hr);
    gs_gpu_destroy(g);
    return 0;
}

/* DHD_RENDER=native draws with the presentation device; the exact backend gets a device of
   its own for the Z probe. Without it, or if that fails, the exact backend presents. */
int gs_gpu_init(Gs *g, ID3D11Device *device, ID3D11DeviceContext *context)
{
    const char *render = getenv("DHD_RENDER");
    if (render && !strcmp(render, "native")) {
        static const D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
        ID3D11Device *exact_device = NULL;
        ID3D11DeviceContext *exact_context = NULL;
        if (SUCCEEDED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, levels, 2, D3D11_SDK_VERSION,
                                        &exact_device, NULL, &exact_context))) {
            int ok = exact_init(g, exact_device, exact_context);
            ID3D11DeviceContext_Release(exact_context); ID3D11Device_Release(exact_device);
            if (ok && native_init(g, g->backend, device, context)) return 1;
            if (ok) gs_gpu_destroy(g);
        }
        fprintf(stderr, "gs: native renderer unavailable; using the exact renderer\n");
    }
    return exact_init(g, device, context);
}
