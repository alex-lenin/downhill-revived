/* Diagnostic replay of our legacy GS captures, without running the EE/VU. */
#define COBJMACROS
#include "gpu_win32.h"
#include "present.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *input;
static void read_bytes(void *p, size_t size)
{
    if (fread(p, 1, size, input) != size) { fprintf(stderr, "Truncated GS dump\n"); exit(1); }
}
static uint32_t read32(void) { uint32_t v; read_bytes(&v, 4); return v; }
static uint64_t read64(void) { uint64_t v; read_bytes(&v, 8); return v; }

static void registers(void)
{
    uint8_t priv[8192]; read_bytes(priv, sizeof priv);
    for (unsigned i = 0; i < sizeof gs.privileged / sizeof *gs.privileged; i++)
        memcpy(gs.privileged + i, priv + i * 16, 8);
}

static void freeze(void)
{
    static const unsigned env[] = {GS_PRIM, GS_PRMODECONT, GS_TEXCLUT, GS_SCANMSK,
        GS_TEXA, GS_FOGCOL, GS_DIMX, GS_DTHE, GS_COLCLAMP, GS_PABE, GS_BITBLTBUF,
        GS_TRXDIR, GS_TRXPOS, GS_TRXREG, GS_TRXREG};
    static const unsigned context[] = {GS_XYOFFSET_1, GS_TEX0_1, GS_TEX1_1, GS_CLAMP_1,
        GS_MIPTBP1_1, GS_MIPTBP2_1, GS_SCISSOR_1, GS_ALPHA_1, GS_TEST_1,
        GS_FBA_1, GS_FRAME_1, GS_ZBUF_1};
    uint32_t crc = read32(), size = read32(), version = read32(); (void)crc;
    if (size != 4194752 || version != 8) { fprintf(stderr, "Expected native version-8 capture\n"); exit(1); }
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
}

static void image(const char *prefix, unsigned field)
{
    static uint32_t pixels[1024 * 1024]; unsigned w, h;
    if (!gs_gpu_display(&gs, &w, &h) || !gs_gpu_capture(&gs, pixels)) return;
    char name[1024]; snprintf(name, sizeof name, "%s-%03u.ppm", prefix, field);
    FILE *f = fopen(name, "wb"); if (!f) { perror(name); exit(1); }
    fprintf(f, "P6\n%u %u\n255\n", w,h);
    for (unsigned i = 0; i < w*h; i++) {
        uint8_t rgb[3] = {(uint8_t)pixels[i], (uint8_t)(pixels[i] >> 8), (uint8_t)(pixels[i] >> 16)};
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

int main(int argc, char **argv)
{
    if (argc != 3) { fprintf(stderr, "Usage: replay.exe capture.gs output-prefix\n"); return 1; }
    input = fopen(argv[1], "rb"); if (!input) { perror(argv[1]); return 1; }
    gs_reset(&gs); freeze();
    ID3D11Device *device; ID3D11DeviceContext *context; D3D_FEATURE_LEVEL level;
    if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
        D3D11_SDK_VERSION, &device, &level, &context)) || !gs_gpu_init(&gs, device, context)) return 1;
    unsigned fields = 0; int type;
    while ((type = fgetc(input)) != EOF) {
        if (type == 0) {
            unsigned path = fgetc(input); uint32_t size = read32();
            if (path < 1 || path > 3 || (size & 15)) return 1;
            uint32_t *packet = malloc(size); if (!packet) return 1;
            read_bytes(packet, size); gs_feed(&gs, path, packet, size / 4); free(packet);
        } else if (type == 1) {
            unsigned field = fgetc(input);
            gs.privileged[GS_CSR / 16] = (gs.privileged[GS_CSR / 16] & ~(UINT64_C(1) << 13)) | (uint64_t)field << 13;
            image(argv[2], fields++);
        } else if (type == 2) {
            unsigned count = read32(); uint8_t *data = malloc((size_t)count * 16);
            if (!data) return 1; gs_read_image(&gs, data, count * 16); free(data);
        } else if (type == 3) registers();
        else { fprintf(stderr, "Unknown GS record %d\n", type); return 1; }
    }
    fclose(input); gs_gpu_destroy(&gs);
    ID3D11DeviceContext_Release(context); ID3D11Device_Release(device);
    printf("Replayed %u fields\n", fields); return 0;
}
