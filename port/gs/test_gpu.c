#define COBJMACROS
#include "gpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned failures;
#define CHECK(c) do { if (!(c)) { printf("FAIL GPU line %d: %s\n", __LINE__, #c); failures++; } } while (0)

static void vertex(unsigned x, unsigned y, unsigned z)
{
    gs_write(&gs, GS_XYZ2, x * 16 | (uint64_t)(y * 16) << 16 | (uint64_t)z << 32);
}
static void sprite(unsigned prim, uint32_t color, unsigned x, unsigned y, unsigned w, unsigned h, unsigned z)
{
    gs_write(&gs, GS_PRIM, prim | 6);
    gs_write(&gs, GS_RGBAQ, color | UINT64_C(0x3F800000) << 32);
    vertex(x,y,z); vertex(x+w,y+h,z);
    gs.sync(&gs);
}
static uint32_t at(unsigned x, unsigned y) { return gs_read_pixel(&gs, 0, 0, 1, x, y); }

static void mip_sample(uint64_t tex1, float q, unsigned x)
{
    uint32_t qb; memcpy(&qb, &q, 4);
    gs_write(&gs, GS_TEX1_1, tex1);
    gs_write(&gs, GS_PRIM, 6 | 16);
    gs_write(&gs, GS_RGBAQ, (uint64_t)qb << 32 | 0x80808080);
    /* ST/Q selects (1,1) in the 64x64 base texture. */
    float st = q / 64; uint32_t sb; memcpy(&sb, &st, 4);
    gs_write(&gs, GS_ST, sb | (uint64_t)sb << 32);
    vertex(x,0,1); vertex(x+1,1,1);
    gs.sync(&gs);
}

static void mipmapping(void)
{
    gs_write(&gs, GS_FRAME_1, 1u << 16);
    gs_write(&gs, GS_TEST_1, 0); gs_write(&gs, GS_CLAMP_1, 5);
    gs_write(&gs, GS_SCISSOR_1, UINT64_C(63) << 16 | UINT64_C(63) << 48);
    uint64_t mip[2] = {0,0};
    for (unsigned level = 0; level < 7; level++) {
        unsigned bp = 1024 + level * 256, bw = level & 1 ? 3 : 2, size = 64 >> level;
        for (unsigned y = 0; y < size; y++) for (unsigned x = 0; x < size; x++)
            gs_write_pixel(&gs, 0, bp,bw,x,y,0x80000000 | level * 32);
        if (level) mip[(level-1)/3] |= (uint64_t)(bp | bw << 14) << (((level-1)%3)*20);
    }
    gs_write(&gs, GS_TEX0_1, 1024 | 2u << 14 | 6u << 26 | UINT64_C(6) << 30 |
             UINT64_C(1) << 34 | UINT64_C(1) << 35);
    gs_write(&gs, GS_MIPTBP1_1, mip[0]); gs_write(&gs, GS_MIPTBP2_1, mip[1]);
    for (unsigned mode = 0; mode < 6; mode++) for (unsigned level = 0; level < 7; level++) {
        mip_sample(1 | 6u << 2 | mode << 6 | (uint64_t)(level * 16) << 32, 1, level);
        CHECK(at(level,0) == (0x80000000 | (mode < 2 ? 0 : level * 32)));
    }
    /* Half-level ties round up for MMIN 2/4 and blend for MMIN 3/5. */
    for (unsigned mode = 2; mode < 6; mode++) {
        mip_sample(1 | 6u << 2 | mode << 6 | UINT64_C(40) << 32, 1, mode);
        CHECK(at(mode,0) == (0x80000000 | (mode & 1 ? 80 : 96)));
    }
    for (unsigned l = 0; l < 3; l++) {
        mip_sample(6u << 2 | 2u << 6 | l << 19 | UINT64_C(16) << 32, .5f, l);
        CHECK(at(l,0) == (0x80000000 | ((1u << l) + 1) * 32));
    }
    /* Negative Q has the same LOD, and MXL clamps the selected level. */
    mip_sample(2u << 2 | 2u << 6, -.125f, 0); CHECK(at(0,0) == 0x80000040);
    /* Opposite MMAG/MMIN choices, including negative signed K. */
    for (unsigned y = 0; y < 2; y++) for (unsigned x = 0; x < 2; x++)
        gs_write_pixel(&gs, 0,1024,2,x,y,0x80000000 | (y*2+x)*64);
    mip_sample(1 | 32 | UINT64_C(0xFF0) << 32, 1, 0); CHECK(at(0,0) == 0x80000060);
    mip_sample(1 | 1u << 2 | 1u << 6 | UINT64_C(16) << 32, 1, 0); CHECK(at(0,0) == 0x80000060);
    mip_sample(1 | 1u << 2 | 32 | UINT64_C(16) << 32, 1, 0); CHECK(at(0,0) == 0x800000C0);
    mip_sample(1 | 32 | UINT64_C(16) << 32, 1, 0); CHECK(at(0,0) == 0x80000060);
    /* A mip-only GPU write must invalidate the texture snapshot. Region
       clamp/repeat bounds halve along with the mip dimensions. */
    for (unsigned mode = 2; mode <= 3; mode++) {
        gs_write(&gs, GS_FRAME_1, 40 | 2u << 16);
        gs_write(&gs, GS_PRIM, 0); gs_write(&gs, GS_RGBAQ, 0x80765400 | mode);
        vertex(20,12,1);
        gs_write(&gs, GS_FRAME_1, 1u << 16);
        gs_write(&gs, GS_CLAMP_1, mode | mode << 2 | UINT64_C(40) << 4 | UINT64_C(40) << 14 |
                 UINT64_C(24) << 24 | UINT64_C(24) << 34);
        /* Level one points at the render target just written. */
        gs_write(&gs, GS_MIPTBP1_1, 1280 | 2u << 14);
        mip_sample(1 | 1u << 2 | 2u << 6 | UINT64_C(16) << 32, 1, 0);
        CHECK(at(0,0) == (0x80765400 | mode));
    }
    gs_write(&gs, GS_CLAMP_1, 0); gs_write(&gs, GS_TEX1_1, 0);
    /* MTBA ignores rectangular TH and explicit TBW; TEX2 does not trigger it. */
    gs_write(&gs, GS_TEX1_1, 512);
    gs_write(&gs, GS_TEX0_1, 4096 | 7u << 14 | 6u << 26 | UINT64_C(2) << 30);
    CHECK(gs.reg[GS_MIPTBP1_1] == ((UINT64_C(4160) | 1u << 14) |
          (UINT64_C(4176) | 1u << 14) << 20 | (UINT64_C(4180) | 1u << 14) << 40));
    gs_write(&gs, GS_TEX1_1, 0);
}

static void depth_precision(void)
{
    const unsigned formats[] = {48,49,50,58};
    const uint32_t bases[] = {0xFFFFFF00,0xFFFFFFF0,0x7FFFFF00,0x00FFFFF0};
    gs_write(&gs, GS_FRAME_1, 1u << 16);
    gs_write(&gs, GS_SCISSOR_1, UINT64_C(63) << 16 | UINT64_C(63) << 48);
    for (unsigned fi = 0; fi < 4; fi++) for (unsigned bi = 0; bi < 4; bi++) {
        unsigned psm = formats[fi], mask = psm == 49 ? 0xFFFFFF : fi >= 2 ? 0xFFFF : UINT32_MAX;
        uint32_t base = bases[bi];
        if (bi < 3) base &= mask;
        gs_write(&gs, GS_ZBUF_1, 2 | (uint64_t)(psm & 15) << 24);
        for (unsigned pass = 0; pass < 3; pass++) {
            gs_write(&gs, GS_TEST_1, 1u << 16 | (pass ? pass == 1 ? 2u : 3u : 1u) << 17);
            gs_write(&gs, GS_PRIM, 3 | (pass == 1 ? 8 : 0));
            gs_write(&gs, GS_RGBAQ, 0x80442200 | pass);
            vertex(0,0,base); vertex(16,0,base+8); vertex(0,16,base+12);
            gs.sync(&gs);
            for (unsigned y = 1; y < 7; y++) for (unsigned x = 1; x < 7; x++) {
                uint32_t expected = base + (2*x + 3*y)/4;
                if (expected > mask) expected = mask;
                CHECK(gs_read_pixel(&gs, psm, 64, 1, x,y) == expected);
                /* Identical Z passes GEQUAL regardless of shading mode, then
                   fails GREATER without altering either surface. */
                CHECK(at(x,y) == (0x80442200 | (pass ? 1 : 0)));
            }
        }
        gs_write(&gs, GS_ZBUF_1, 2 | (uint64_t)(psm & 15) << 24 | UINT64_C(1) << 32);
        gs_write(&gs, GS_TEST_1, 1u << 16 | 1u << 17);
        gs_write(&gs, GS_PRIM, 3); vertex(0,0,1); vertex(16,0,2); vertex(0,16,3); gs.sync(&gs);
        CHECK(gs_read_pixel(&gs, psm, 64, 1, 2,3) == (base + 3 > mask ? mask : base + 3));
    }
    gs_write(&gs, GS_ZBUF_1, 2); gs_write(&gs, GS_TEST_1, 1u << 16 | 1u << 17);
    gs_write(&gs, GS_PRIM, 1); vertex(0,0,0xFFFFFFF0); vertex(15,0,0xFFFFFFFF); gs.sync(&gs);
    for (unsigned x = 0; x < 15; x++) CHECK(gs_read_pixel(&gs, 48,64,1,x,0) == 0xFFFFFFF0+x);
    gs_write(&gs, GS_TEST_1, 0);
}

static void fog_blend(void)
{
    const uint32_t colors[] = {0x801478C8,0x80A05028};
    gs_write(&gs, GS_FRAME_1, 1u << 16); gs_write(&gs, GS_TEST_1, 0);
    gs_write(&gs, GS_FOGCOL, 0xA05028);
    for (unsigned group = 0; group < 2; group++) {
        gs_write(&gs, GS_PRIM, 32); gs_write(&gs, GS_RGBAQ, colors[group]);
        for (unsigned f = 0; f < 256; f++) {
            gs_write(&gs, GS_FOG, (uint64_t)f << 56);
            vertex(f%64, group*4+f/64, 1);
        }
        gs.sync(&gs);
        for (unsigned f = 0; f < 256; f++) {
            uint32_t expected = 0x80000000;
            for (unsigned channel = 0; channel < 3; channel++) {
                int src = colors[group] >> (channel*8) & 255, fc = 0xA05028 >> (channel*8) & 255;
                expected |= (uint32_t)(fc + (((src-fc)*(int)f) >> 8)) << (channel*8);
            }
            CHECK(at(f%64, group*4+f/64) == expected);
        }
    }
    CHECK(at(0,0) == 0x80A05028); /* F=0 preserves the fog color. */
    CHECK(at(63,3) == 0x801477C7); /* F=255; negative blue difference rounds down. */
    CHECK(at(1,0) == 0x809F5028); /* Negative difference uses arithmetic shift. */
    CHECK(at(63,7) == 0x80A05028); /* Equal colors remain equal for every F. */
}

static void batch_ordering(ID3D11Device *device, ID3D11DeviceContext *context)
{
    static uint32_t reference[GS_VRAM_SIZE / 4];
    for (unsigned pass = 0; pass < 2; pass++) {
        gs_gpu_destroy(&gs); gs_reset(&gs);
        _putenv_s("DHD_GS_BATCH", pass ? "256" : "1");
        CHECK(gs_gpu_init(&gs, device, context));
        gs_write(&gs, GS_FRAME_1, 2u << 16);
        gs_write(&gs, GS_ZBUF_1, 16);
        gs_write(&gs, GS_SCISSOR_1, UINT64_C(127) << 16 | UINT64_C(127) << 48);
        gs_write(&gs, GS_COLCLAMP, 1);
        gs_write(&gs, GS_TEST_1, 1u << 16 | 2u << 17);
        gs_write(&gs, GS_ALPHA_1, 1u << 2 | 1u << 6);
        unsigned random = 19;
        for (unsigned i = 0; i < 1024; i++) {
            if (i == 512) {
                /* An unrelated CPU upload must survive a later overlapping
                   write's GPU readback, even before either CPU page uploads. */
                gs_write_pixel(&gs, 0, 15000, 2, 0,0,0x80765432);
                gs_write_pixel(&gs, 0, 0, 2, 12,12,0x80123456);
                CHECK(gs_read_pixel(&gs, 0, 15000, 2, 0,0) == 0x80765432);
            }
            if (i == 768) gs_write(&gs, GS_FRAME_1, 2u << 16 | 2u << 24);
            if (i == 900) {  /* Masked Z can still read another pixel's color. */
                gs_write(&gs, GS_FRAME_1, 2u << 16);
                gs_write(&gs, GS_ZBUF_1, UINT64_C(1) << 32);
            }
            random = random * 1664525u + 1013904223u;
            unsigned kind = i % 4 == 0 ? 6 : i % 4 == 1 ? 0 : i % 4 == 2 ? 1 : 3;
            unsigned x = random & 127, y = random >> 8 & 127, z = random >> 16;
            gs_write(&gs, GS_PRIM, kind | 64 | 8);
            gs_write(&gs, GS_RGBAQ, random);
            vertex(x,y,z);
            if (kind) vertex(x+17,y+9,z+1);
            if (kind == 3) vertex(x+4,y+25,z+2);
        }
        gs.sync(&gs);
        CHECK(gs_read_pixel(&gs, 0, 15000, 2, 0,0) == 0x80765432);
        if (!pass) memcpy(reference, gs.vram, sizeof reference);
        else CHECK(memcmp(reference, gs.vram, sizeof reference) == 0);
    }
    _putenv_s("DHD_GS_BATCH", "");
}

static void scanout(void)
{
    uint32_t pixels[80 * 66];
    const unsigned formats[] = {0,1,2,10};
    gs.sync(&gs);
    for (unsigned f = 0; f < 4; f++) for (unsigned stride = 1; stride <= 4; stride++) {
        unsigned psm = formats[f], w = 0, h = 0;
        for (unsigned i = 0; i < GS_VRAM_SIZE / 4; i++) gs.vram[i] = i * 1234567u;
        gs.pixels_written++;
        memset(gs.host_dirty, 255, sizeof gs.host_dirty);
        gs_priv_write(&gs, GS_PMODE, 2);
        gs_priv_write(&gs, GS_DISPFB2, 511 | (uint64_t)stride << 9 | (uint64_t)psm << 15 |
                      UINT64_C(2040) << 32 | UINT64_C(2040) << 43);
        gs_priv_write(&gs, GS_DISPLAY2, UINT64_C(79) << 32 | UINT64_C(65) << 44);
        for (unsigned field = 0; field < 3; field++) {
            gs_priv_write(&gs, GS_SMODE2, field ? 3 : 0);
            if (field == 2) {
                for (unsigned i = 0; i < GS_VRAM_SIZE / 4; i++) gs.vram[i] ^= 0x12345678;
                gs.pixels_written++;
                memset(gs.host_dirty, 255, sizeof gs.host_dirty);
                gs_vblank(&gs);
            }
            CHECK(gs_gpu_display(&gs, &w, &h) && w == 80 && h == 66);
            CHECK(gs_gpu_capture(&gs, pixels));
            for (unsigned y = 0; y < h; y++) for (unsigned x = 0; x < w; x++) {
                uint32_t v = gs_read_pixel(&gs, psm, 511 * 32, stride, (2040+x)&2047, (2040+(y>>(field!=0)))&2047);
                uint32_t expected = gs_color(v, psm, UINT64_C(0x80) << 32) | 0xFF000000;
                CHECK(pixels[y*w+x] == expected);
            }
        }
    }
}

/* TRXDIR 2 on the GPU leaves VRAM exactly as the CPU path does, across formats; rectangles on
   shared pages (here overlapping) stay on the CPU. */
static int (*gpu_copy)(Gs *);
static unsigned gpu_copies;
static int counted_copy(Gs *g) { int done = gpu_copy(g); gpu_copies += done; return done; }
static void local_copies(void)
{
    static uint32_t expected[GS_VRAM_SIZE / 4];
    static const struct { unsigned spsm, sbp, sbw, sx, sy, dpsm, dbp, dbw, dx, dy, w, h; } copies[] = {
        {0, 0, 10, 3, 5, 0, 0x2000, 10, 17, 9, 100, 40},     /* CT32 */
        {1, 0, 10, 0, 0, 1, 0x2000, 10, 64, 32, 64, 32},     /* CT24 keeps the destination alpha */
        {2, 0x800, 4, 7, 1, 2, 0x3000, 4, 1, 70, 90, 50},    /* CT16 */
        {10, 0x800, 4, 0, 0, 2, 0x3000, 4, 5, 5, 33, 17},    /* CT16S to CT16 */
        {19, 0x1000, 2, 9, 3, 19, 0x3800, 2, 2, 66, 120, 70},/* T8 */
        {20, 0x1000, 2, 1, 2, 20, 0x3800, 2, 30, 130, 127, 99}, /* T4 */
        {48, 0x1800, 10, 0, 0, 0, 0x2000, 10, 0, 0, 40, 40}, /* Z32 to CT32 */
        {0, 0, 10, 0, 0, 0, 0, 10, 4, 4, 32, 16},            /* overlapping: the CPU's order */
    };
    int (*hook)(Gs *) = gs.local_copy;
    gpu_copy = hook;
    for (unsigned n = 0; n < sizeof copies / sizeof *copies; n++) {
        for (unsigned pass = 0; pass < 2; pass++) {
            for (unsigned i = 0; i < GS_VRAM_SIZE / 4; i++) gs.vram[i] = i * 2654435761u ^ n;
            gs.pixels_written++;
            memset(gs.host_dirty, 255, sizeof gs.host_dirty);
            gs.local_copy = pass ? NULL : counted_copy;
            gs_write(&gs, GS_BITBLTBUF, copies[n].sbp | (uint64_t)copies[n].sbw << 16 | (uint64_t)copies[n].spsm << 24 |
                     (uint64_t)copies[n].dbp << 32 | (uint64_t)copies[n].dbw << 48 | (uint64_t)copies[n].dpsm << 56);
            gs_write(&gs, GS_TRXPOS, copies[n].sx | (uint64_t)copies[n].sy << 16 | (uint64_t)copies[n].dx << 32 |
                     (uint64_t)copies[n].dy << 48);
            gs_write(&gs, GS_TRXREG, copies[n].w | (uint64_t)copies[n].h << 32);
            gs_write(&gs, GS_TRXDIR, 2);
            gs.sync(&gs);
            if (!pass) memcpy(expected, gs.vram, sizeof expected);
            else if (memcmp(expected, gs.vram, sizeof expected)) { printf("FAIL local copy %u\n", n); failures++; }
        }
    }
    CHECK(gpu_copies == sizeof copies / sizeof *copies - 1);
    gs.local_copy = hook;
}

/* gs_defer_read_image against gs_read_image on the same VRAM state, with draws over the pixels
   before the deferred bytes are written. */
static void unsynced_sprite(uint32_t color, unsigned x, unsigned y, unsigned w, unsigned h, unsigned z)
{
    gs_write(&gs, GS_PRIM, 6);
    gs_write(&gs, GS_RGBAQ, color | UINT64_C(0x3F800000) << 32);
    vertex(x,y,z); vertex(x+w,y+h,z);
}
static void start_read(unsigned psm, unsigned bp, unsigned x, unsigned y)
{
    gs_write(&gs, GS_BITBLTBUF, bp | 1u << 16 | (uint64_t)psm << 24);
    gs_write(&gs, GS_TRXPOS, x | (uint64_t)y << 16);
    gs_write(&gs, GS_TRXREG, 4 | UINT64_C(4) << 32);
    gs_write(&gs, GS_TRXDIR, 1);
}
static void deferred_reads(void)
{
    uint8_t depth[64], color[64], depth_now[64], color_now[64];
    gs_write(&gs, GS_ZBUF_1, 1); /* Z32 at page 1, written */
    gs_write(&gs, GS_TEST_1, 1u << 16 | 1u << 17); /* Z always */
    unsynced_sprite(0x80123456, 0,0,16,16, 0x1234);
    memset(depth, 0xCC, sizeof depth); memset(color, 0xCC, sizeof color);
    start_read(48, 32, 2,2);  /* Z32 at ZBUF page 1 */
    CHECK(gs_defer_read_image(&gs, depth, sizeof depth) == sizeof depth);
    CHECK(gs.transfer_count == 16 && !gs.transfer_have && depth[0] == 0xCC);
    gs.transfer_count = 0;
    CHECK(gs_read_image(&gs, depth_now, sizeof depth_now) == sizeof depth_now);
    unsynced_sprite(0x80654321, 0,0,16,16, 0x4321);
    start_read(48, 32, 1,1);  /* the page changed since it was captured */
    CHECK(gs_defer_read_image(&gs, depth, sizeof depth) == 0 && !gs.transfer_count);
    start_read(0, 0, 3,5);    /* a page not captured yet */
    CHECK(gs_defer_read_image(&gs, color, sizeof color) == sizeof color);
    gs.transfer_count = 0;
    CHECK(gs_read_image(&gs, color_now, sizeof color_now) == sizeof color_now);
    unsynced_sprite(0x80000000, 0,0,16,16, 0);
    CHECK(gs_reads_pending(&gs));
    gs_settle_reads(&gs);
    CHECK(!gs_reads_pending(&gs));
    CHECK(!memcmp(depth, depth_now, sizeof depth) && depth[0] == 0x34 && depth[1] == 0x12);
    CHECK(!memcmp(color, color_now, sizeof color) && color[0] == 0x21 && color[2] == 0x65);
}

int main(void)
{
    ID3D11Device *device = NULL;
    ID3D11DeviceContext *context = NULL;
    D3D_FEATURE_LEVEL level;
    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &device, &level, &context);
    if (FAILED(hr)) { printf("FAIL D3D11 WARP %08lX\n", hr); return 1; }
    gs_reset(&gs);
    if (!gs_gpu_init(&gs, device, context)) return 1;
    gs_write(&gs, GS_FRAME_1, 1u << 16);
    gs_write(&gs, GS_SCISSOR_1, UINT64_C(63) << 16 | UINT64_C(63) << 48);
    gs_write(&gs, GS_COLCLAMP, 1);
    gs_write(&gs, GS_ZBUF_1, 1); /* 8192-byte page */
    gs_write(&gs, GS_TEST_1, 1u << 16 | 1u << 17); /* Z always */
    sprite(0, 0x80102040, 2,3,8,6,100);
    CHECK(at(2,3) == 0x80102040 && at(9,8) == 0x80102040 && at(10,8) == 0 && at(1,3) == 0);
    CHECK(gs_read_pixel(&gs, 48, 32, 1, 5,5) == 100);
    gs_write(&gs, GS_TEST_1, 1u << 16 | 2u << 17); /* GEQUAL */
    sprite(0, 0x80404040, 3,4,2,2,99);
    CHECK(at(3,4) == 0x80102040);
    sprite(0, 0x80404040, 3,4,2,2,101);
    CHECK(at(3,4) == 0x80404040);
    /* (Cs - Cd) * As / 128 + Cd. */
    gs_write(&gs, GS_ALPHA_1, 1u << 2 | 1u << 6);
    sprite(64, 0x406080A0, 3,4,1,1,102);
    CHECK(at(3,4) == 0x40506070);
    /* Alpha NEVER + RGB_ONLY preserves destination alpha and Z. */
    gs_write(&gs, GS_TEST_1, 1 | 3u << 12 | 1u << 16 | 1u << 17);
    sprite(0, 0x99123456, 3,4,1,1,200);
    CHECK(at(3,4) == 0x40123456 && gs_read_pixel(&gs, 48, 32, 1, 3,4) == 102);
    gs_write(&gs, GS_TEST_1, 1u << 16 | 1u << 17);
    /* Textured sprite in UV mode, nearest DECAL, preserving uploaded VRAM. */
    gs_write_pixel(&gs, 0, 128, 1, 0,0,0x80776655);
    gs_write(&gs, GS_TEX0_1, 128 | 1u << 14 | UINT64_C(1) << 34 | UINT64_C(1) << 35);
    gs_write(&gs, GS_UV, 0);
    sprite(16 | 256, 0x80808080, 12,12,4,4,1);
    CHECK(at(12,12) == 0x80776655 && at(15,15) == 0x80776655);
    /* Sampling a framebuffer sees a prior GPU draw without a CPU upload. */
    gs_write(&gs, GS_TEX0_1, 1u << 14 | 6u << 26 | UINT64_C(6) << 30 | UINT64_C(1) << 34 | UINT64_C(1) << 35);
    gs_write(&gs, GS_UV, 12u * 16 | (uint64_t)(12u * 16) << 16);
    sprite(16 | 256, 0x80808080, 20,20,2,2,1);
    CHECK(at(20,20) == 0x80776655);
    /* Triangle coverage, both orientations, flat provoking color. */
    gs_write(&gs, GS_PRIM, 3); gs_write(&gs, GS_RGBAQ, 0x80ABCDEF);
    vertex(30,30,1); vertex(38,30,1); vertex(30,38,1); gs.sync(&gs);
    CHECK(at(31,31) == 0x80ABCDEF && at(37,37) == 0);
    gs_write(&gs, GS_PRIM, 3); gs_write(&gs, GS_RGBAQ, 0x80335577);
    vertex(40,40,1); vertex(40,48,1); vertex(48,40,1); gs.sync(&gs);
    CHECK(at(41,41) == 0x80335577);
    /* Scissor clips the sprite to inclusive right/bottom bounds. */
    gs_write(&gs, GS_SCISSOR_1, 50 | UINT64_C(51) << 16 | UINT64_C(50) << 32 | UINT64_C(51) << 48);
    sprite(0, 0x80112233, 48,48,8,8,1);
    CHECK(at(50,50) == 0x80112233 && at(51,51) == 0x80112233 && at(52,52) == 0);
    gs_write(&gs, GS_SCISSOR_1, UINT64_C(63) << 16 | UINT64_C(63) << 48);
    gs_write(&gs, GS_TEST_1, 0);
    gs_write(&gs, GS_PRIM, 0); gs_write(&gs, GS_RGBAQ, 0x80443322);
    vertex(60,3,1); gs.sync(&gs);
    CHECK(at(60,3) == 0x80443322 && at(61,3) == 0);
    gs_write(&gs, GS_PRIM, 1);
    vertex(2,55,1); vertex(9,55,1); gs.sync(&gs);
    CHECK(at(3,55) == 0x80443322 && at(9,55) == 0);
    gs_write(&gs, GS_PRIM, 4);
    vertex(2,40,1); vertex(10,40,1); vertex(2,48,1); vertex(10,48,1); gs.sync(&gs);
    CHECK(at(3,41) == 0x80443322 && at(8,46) == 0x80443322);
    gs_write(&gs, GS_PRIM, 5);
    vertex(15,40,1); vertex(23,40,1); vertex(23,48,1); vertex(15,48,1); gs.sync(&gs);
    CHECK(at(16,46) == 0x80443322 && at(21,41) == 0x80443322);
    gs_write(&gs, GS_FOGCOL, 0x00FF00); gs_write(&gs, GS_FOG, 0);
    sprite(32, 0x80776655, 30,50,2,2,1);
    CHECK(at(30,50) == 0x8000FF00);
    /* Indexed sampling uses a loaded CLUT, not live palette memory. */
    gs_write_pixel(&gs, 19, 512, 2, 0,0,5);
    gs_write_pixel(&gs, 0, 640, 1, 5,0,0x80445566);
    gs_write(&gs, GS_TEX0_1, 512 | 2u << 14 | 19u << 20 | UINT64_C(640) << 37 |
             UINT64_C(1) << 61 | UINT64_C(1) << 34 | UINT64_C(1) << 35);
    gs_write(&gs, GS_UV, 0);
    sprite(16 | 256, 0x80808080, 32,50,2,2,1);
    CHECK(at(32,50) == 0x80445566);
    /* A GPU-written CLUT must be read back before CLD, even when other
       palettes were served from still-current CPU pages. */
    gs_write(&gs, GS_PRIM, 0); gs_write(&gs, GS_RGBAQ, 0x80773311);
    vertex(5,0,1);
    gs_write(&gs, GS_TEXCLUT, 1);
    gs_write(&gs, GS_TEX0_1, 512 | 2u << 14 | 19u << 20 | UINT64_C(1) << 55 |
             UINT64_C(1) << 61 | UINT64_C(1) << 34 | UINT64_C(1) << 35);
    sprite(16 | 256, 0x80808080, 32,50,2,2,1);
    CHECK(at(32,50) == 0x80773311);
    /* Bilinear STQ sampling at the centre of four texels, with Q=2. */
    for (unsigned y = 0; y < 2; y++) for (unsigned x = 0; x < 2; x++)
        gs_write_pixel(&gs, 0, 256, 1, x,y,0x80000000 | (y * 2 + x) * 64);
    gs_write(&gs, GS_TEX0_1, 256 | 1u << 14 | 1u << 26 | UINT64_C(1) << 30 |
             UINT64_C(1) << 34 | UINT64_C(1) << 35);
    gs_write(&gs, GS_TEX1_1, 32); gs_write(&gs, GS_PRIM, 6 | 16);
    gs_write(&gs, GS_RGBAQ, UINT64_C(0x40000000) << 32 | 0x80808080);
    gs_write(&gs, GS_ST, UINT64_C(0x3F8000003F800000));
    vertex(35,50,1); vertex(37,52,1); gs.sync(&gs);
    CHECK(at(35,50) == 0x80000060);
    /* Atlas tiles away from the origin exercise indexed page/column swizzles. */
    gs_write(&gs, GS_TEX1_1, 0);
    const unsigned indexed[] = {19,20,27,36,44};
    for (unsigned fi = 0; fi < sizeof indexed / sizeof *indexed; fi++) {
        unsigned format = indexed[fi];
        unsigned mask = format == 19 || format == 27 ? 255 : 15;
        for (unsigned i = 0; i < 256; i++) {
            gs.clut[i] = (uint16_t)(i * 257);
            gs.clut[i + 256] = (uint16_t)(0x8000 | i);
        }
        for (unsigned y = 0; y < 64; y++) for (unsigned x = 0; x < 64; x++)
            gs_write_pixel(&gs, format, 512, 8, x+128,y+256, ((x * 13) ^ (y * 7) ^ (y >> 2)) & mask);
        gs_write(&gs, GS_TEX0_1, 512 | 8u << 14 | (uint64_t)format << 20 | 9u << 26 |
                 UINT64_C(9) << 30 | UINT64_C(1) << 34 | UINT64_C(1) << 35);
        gs_write(&gs, GS_PRIM, 6 | 16 | 256);
        gs_write(&gs, GS_UV, 128u * 16 + 8 | (uint64_t)(256u * 16 + 8) << 16); vertex(0,0,1);
        gs_write(&gs, GS_UV, 192u * 16 + 8 | (uint64_t)(320u * 16 + 8) << 16); vertex(64,64,1);
        gs.sync(&gs);
        for (unsigned y = 0; y < 64; y++) for (unsigned x = 0; x < 64; x++) {
            unsigned index = ((x * 13) ^ (y * 7) ^ (y >> 2)) & mask;
            CHECK(at(x,y) == (0x80000000 | index * 0x010101));
        }
    }
    /* Full-range GS coordinates overflow a signed 32-bit edge determinant. */
    gs_write(&gs, GS_PRIM, 3); gs_write(&gs, GS_RGBAQ, 0x80446688);
    vertex(0,0,1); vertex(4095,0,1); vertex(0,4095,1); gs.sync(&gs);
    CHECK(at(1,1) == 0x80446688 && at(63,63) == 0x80446688);
    gs_write(&gs, GS_PRIM, 3); gs_write(&gs, GS_RGBAQ, 0x80112233);
    vertex(0,0,1); vertex(0,4095,1); vertex(4095,0,1); gs.sync(&gs);
    CHECK(at(1,1) == 0x80112233 && at(63,63) == 0x80112233);
    /* Constant Z32 must survive without conversion through float (24-bit precision). */
    gs_write(&gs, GS_TEST_1, 1u << 16 | 1u << 17);
    gs_write(&gs, GS_PRIM, 3);
    vertex(0,0,0xFFFFFFFF); vertex(16,0,0xFFFFFFFF); vertex(0,16,0xFFFFFFFF); gs.sync(&gs);
    CHECK(gs_read_pixel(&gs, 48, 32, 1, 2,2) == 0xFFFFFFFF);
    gs_write(&gs, GS_TEST_1, 0);
    /* Atomic halfword writes and alpha preservation in a 16-bit target. */
    gs_write(&gs, GS_FRAME_1, 2 | 1u << 16 | 2u << 24);
    sprite(0, 0x800000FF, 0,0,16,16,1);
    for (unsigned y = 0; y < 16; y++) for (unsigned x = 0; x < 16; x++)
        CHECK(gs_read_pixel(&gs, 2, 64, 1, x,y) == 0x801F);
    gs_write(&gs, GS_FRAME_1, 2 | 1u << 16 | 2u << 24 | UINT64_C(0xFF000000) << 32);
    sprite(0, 0x0000FF00, 0,0,16,16,1);
    CHECK(gs_read_pixel(&gs, 2, 64, 1, 7,7) == 0x83E0);
    /* Region clamp/repeat can sample outside TEX0's nominal dimensions.
       Write the source on the GPU, then sample it without a CPU barrier. */
    gs_write(&gs, GS_FRAME_1, 10u << 16);
    gs_write(&gs, GS_SCISSOR_1, UINT64_C(639) << 16 | UINT64_C(63) << 48);
    gs_write(&gs, GS_TEX0_1, 10u << 14 | UINT64_C(1) << 34 | UINT64_C(1) << 35);
    gs_write(&gs, GS_UV, 0);
    for (unsigned mode = 2; mode <= 3; mode++) {
        gs_write(&gs, GS_PRIM, 0); gs_write(&gs, GS_RGBAQ, 0x80554400 | mode);
        vertex(300,10,1);
        gs_write(&gs, GS_CLAMP_1, mode | (uint64_t)mode << 2 | UINT64_C(300) << 4 |
                 UINT64_C(300) << 14 | UINT64_C(10) << 24 | UINT64_C(10) << 34);
        sprite(16 | 256, 0x80808080, 0,0,1,1,1);
        CHECK(gs_read_pixel(&gs, 0, 0, 10, 0,0) == (0x80554400 | mode));
    }
    mipmapping();
    depth_precision();
    fog_blend();
    batch_ordering(device, context);
    scanout();
    local_copies();
    deferred_reads();
    gs_gpu_destroy(&gs);
    ID3D11DeviceContext_Release(context); ID3D11Device_Release(device);
    printf("D3D11 WARP raster tests: %u failures\n", failures);
    return failures != 0;
}
