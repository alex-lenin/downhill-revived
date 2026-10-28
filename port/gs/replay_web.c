/* replay.c for the browser: the same legacy GS capture replay, on the web GS backend
   (port/web/gpu_rust_web.c on the Rust renderer, the browser's WebGPU), to compare its VRAM with
   the native replay's (port/tools/web_gs_replay.mjs, finding 084 section 12). The page puts the
   capture into this module's memory and calls replay_web through WebAssembly.promising: every VRAM
   sync suspends in gpu_rust_web.c's web_gpu_await until the GPU has finished. */
#include "gs.h"
#include <stdlib.h>
#include <string.h>

int web_gpu_enable(void);
uint32_t web_display(void);

static const uint8_t *input, *input_end;
static int truncated;
static void read_bytes(void *p, size_t size)
{
    if ((size_t)(input_end - input) < size) { truncated = 1; memset(p, 0, size); input = input_end; return; }
    memcpy(p, input, size); input += size;
}
static uint32_t read32(void) { uint32_t v; read_bytes(&v, 4); return v; }
static uint64_t read64(void) { uint64_t v; read_bytes(&v, 8); return v; }
static int read8(void) { return input < input_end ? *input++ : -1; }

static void registers(void)
{
    static uint8_t priv[8192]; read_bytes(priv, sizeof priv);
    for (unsigned i = 0; i < sizeof gs.privileged / sizeof *gs.privileged; i++)
        memcpy(gs.privileged + i, priv + i * 16, 8);
}

/* replay.c's freeze() */
static int freeze(void)
{
    static const unsigned env[] = {GS_PRIM, GS_PRMODECONT, GS_TEXCLUT, GS_SCANMSK,
        GS_TEXA, GS_FOGCOL, GS_DIMX, GS_DTHE, GS_COLCLAMP, GS_PABE, GS_BITBLTBUF,
        GS_TRXDIR, GS_TRXPOS, GS_TRXREG, GS_TRXREG};
    static const unsigned context[] = {GS_XYOFFSET_1, GS_TEX0_1, GS_TEX1_1, GS_CLAMP_1,
        GS_MIPTBP1_1, GS_MIPTBP2_1, GS_SCISSOR_1, GS_ALPHA_1, GS_TEST_1,
        GS_FBA_1, GS_FRAME_1, GS_ZBUF_1};
    uint32_t crc = read32(), size = read32(), version = read32(); (void)crc;
    if (size != 4194752 || version != 8) return 0;
    for (unsigned i = 0; i < sizeof env / sizeof *env; i++) gs.reg[env[i]] = read64();
    for (unsigned c = 0; c < 2; c++) for (unsigned i = 0; i < sizeof context / sizeof *context; i++)
        gs.reg[context[i] + c] = read64();
    uint64_t v = read64(); gs.vertex.rgba = (uint32_t)v;
    uint32_t q = (uint32_t)(v >> 32); memcpy(&gs.vertex.q, &q, 4);
    read_bytes(&gs.vertex.s, 8); v = read32(); gs.vertex.u = v & 16383; gs.vertex.v = v >> 16 & 16383;
    gs.vertex.fog = read32(); v = read64();
    gs.vertex.x = v & 65535; gs.vertex.y = v >> 16 & 65535; gs.vertex.z = (uint32_t)(v >> 32);
    read64(); read32(); read32(); read_bytes(gs.vram, GS_VRAM_SIZE);
    for (unsigned i = 0; i < 4; i++) {
        uint64_t tag = read64(), regs = read64(); unsigned reg = read32();
        if (i < 3) {
            GsPath *p = gs.path + i; p->left = tag & 32767; p->eop = tag >> 15 & 1;
            p->mode = tag >> 58 & 3; p->nreg = tag >> 60; if (!p->nreg) p->nreg = 16;
            p->regs = regs; p->reg = reg;
        }
    }
    read32(); registers();
    for (unsigned c = 0; c < 2; c++) gs_write(&gs, GS_TEX0_1 + c, gs.reg[GS_TEX0_1 + c]);
    return !truncated;
}

__attribute__((export_name("replay_alloc"))) void *replay_alloc(uint32_t bytes) { return malloc(bytes); }

/* Replays bytes at capture; hashes[field] is that field's whole-VRAM FNV-1a (replay.c's
   vram_hash), for at most max fields. Returns the field count, or -1 on a bad capture. */
__attribute__((export_name("replay_web"))) int replay_web(const uint8_t *capture, uint32_t bytes,
    uint64_t *hashes, unsigned max)
{
    input = capture; input_end = capture + bytes; truncated = 0;
    gs_reset(&gs);
    if (!freeze() || !web_gpu_enable()) return -1;
    unsigned fields = 0; int type;
    while ((type = read8()) >= 0) {
        if (type == 0) {
            unsigned path = read8(); uint32_t size = read32();
            if (path < 1 || path > 3 || (size & 15) || size > (size_t)(input_end - input)) return -1;
            uint32_t *packet = malloc(size); if (!packet) return -1;
            read_bytes(packet, size); gs_feed(&gs, path, packet, size / 4); free(packet);
        } else if (type == 1) {
            unsigned field = read8();
            gs.privileged[GS_CSR / 16] = (gs.privileged[GS_CSR / 16] & ~(UINT64_C(1) << 13)) | (uint64_t)field << 13;
            gs.sync(&gs);
            uint64_t h = 0xCBF29CE484222325ull;
            const uint8_t *p = (const uint8_t *)gs.vram;
            for (size_t i = 0; i < GS_VRAM_SIZE; i++) h = (h ^ p[i]) * 0x100000001B3ull;
            if (fields < max) hashes[fields] = h;
            fields++;
        } else if (type == 2) {
            unsigned count = read32(); uint8_t *data = malloc((size_t)count * 16);
            if (!data) return -1; gs_read_image(&gs, data, count * 16); free(data);
        } else if (type == 3) registers();
        else return -1;
        if (truncated) return -1;
    }
    gs.sync(&gs);
    return (int)fields;
}

/* web_display's register read (port/web/host_graphics.c's, which the replay does not link): the
   one-worker build's, straight from the GS MMIO page. */
uint64_t web_gs_mmio(unsigned addr) { return *(volatile uint64_t *)(uintptr_t)(0x12000000u + addr); }

/* The last picture: web_display reads the privileged registers from the PS2's GS MMIO page
   (0x12000000 in the game's memory), so they are put there first. */
__attribute__((export_name("replay_display"))) uint32_t replay_display(void)
{
    for (unsigned addr = 0; addr <= 0xE0; addr += 16)
        *(volatile uint64_t *)(uintptr_t)(0x12000000u + addr) = gs.privileged[addr / 16];
    return web_display();
}
