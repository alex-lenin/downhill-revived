/* Native transfer ordering, using synthetic solid colours and no game data.
   The presenter, texture replacements and EE scheduler are absent. */
#define COBJMACROS
#include "gpu_win32.h"
#include "present.h"
#include "texpack.h"
#include "font_sharp.h"
#include <windows.h>
uint64_t kernel_vsync_count;
volatile int gs_native_view;
double host_seconds(void) { return GetTickCount64()/1000.0; }
double host_wall_seconds(void) { return host_seconds(); }
void host_thread_name(const char *n) {(void)n;}
void pacing_handed(double *d,double *b) {*d=*b=0;}
void pacing_present(double d,double b) {(void)d;(void)b;}
int host_widescreen_flat_limit(void) {return 1000;}
double host_widescreen_aspect(void) {return 16.0/9;}
UINT gs_swap_chain_flags(void) {return 0;}
void gs_frame_latency_wait(void) {}
HRESULT gs_present(IDXGISwapChain *s) {return IDXGISwapChain_Present(s,0,0);}
int texpack_open(ID3D11Device *d) {return 0;}
int texpack_available(void) {return 0;}
TexPackEntry *texpack_find(const Gs *g,unsigned c) {return NULL;}
ID3D11ShaderResourceView *texpack_view(TexPackEntry *e,uint64_t f) {return NULL;}
void texpack_field(uint64_t f) {}
void texpack_report(void) {}
int font_sharp_on(void) {return 0;}
int font_sharp_match(uint64_t t) {return 0;}
ID3D11ShaderResourceView *font_sharp_view(ID3D11Device *d,const uint32_t *p,unsigned w,unsigned h,uint64_t k) {return NULL;}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum { WIDTH = 512, HEIGHT = 224 };
static unsigned failures;

static void clear(unsigned page, uint32_t color)
{
    gs_write(&gs, GS_FRAME_1, page | 8ULL << 16);
    gs_write(&gs, GS_ZBUF_1, 1ULL << 32);
    gs_write(&gs, GS_TEST_1, 0);
    gs_write(&gs, GS_SCISSOR_1, (WIDTH - 1ULL) << 16 | (HEIGHT - 1ULL) << 48);
    gs_write(&gs, GS_PRIM, 6);
    gs_write(&gs, GS_RGBAQ, color);
    gs_write(&gs, GS_XYZ2, 0);
    gs_write(&gs, GS_XYZ2, WIDTH * 16ULL | (HEIGHT * 16ULL) << 16);
}

static void show(unsigned page, uint32_t color, const char *name)
{
    gs_priv_write(&gs, GS_PMODE, 2);
    gs_priv_write(&gs, GS_SMODE2, 0);
    gs_priv_write(&gs, GS_DISPFB2, page | 8ULL << 9);
    gs_priv_write(&gs, GS_DISPLAY2, (WIDTH - 1ULL) << 32 | (HEIGHT - 1ULL) << 44);
    unsigned w = 0, h = 0;
    ++kernel_vsync_count;
    gs_native_vsync(&gs, NULL, &w, &h, 0);
    uint32_t *pixels = gs_native_picture(&gs, &w, &h);
    unsigned changed = 0;
    if (pixels)
        for (unsigned i = 0; i < w * h; i++)
            changed += (pixels[i] & 0xFFFFFF) != (color & 0xFFFFFF);
    int ok = pixels && w == WIDTH * 2 && h == HEIGHT * 2 && !changed;
    if (!ok) failures++;
    printf("%s: %s (%ux%u, %u incorrect pixels)\n", name, ok ? "PASS" : "FAIL", w, h, changed);
}

static void sample_alpha(unsigned x, unsigned y, unsigned alpha, const char *name)
{
    clear(256, 0);
    /* CSM2 palette: index becomes the red channel, making stored alpha visible. */
    for (unsigned i = 0; i < 256; i++) gs_write_pixel(&gs, GS_PSMCT32, 14000, 4, i, 0, 0x80000000 | i);
    gs_write(&gs, GS_TEXCLUT, 4);
    gs_write(&gs, GS_TEX0_1, 4096ULL | 8ULL << 14 | (uint64_t)GS_PSMT8H << 20 |
             9ULL << 26 | 8ULL << 30 | 1ULL << 35 | 14000ULL << 37 | 1ULL << 55 | 1ULL << 61);
    gs_write(&gs, GS_PRIM, 6 | 16 | 256);
    gs_write(&gs, GS_RGBAQ, 0x3F80000080808080ULL);
    gs_write(&gs, GS_UV, (x * 16 + 8) | (uint64_t)(y * 16 + 8) << 16);
    gs_write(&gs, GS_XYZ2, 0);
    gs_write(&gs, GS_XYZ2, WIDTH * 16ULL | (HEIGHT * 16ULL) << 16);
    show(256, alpha, name);
}

int main(void)
{
    gs_reset(&gs);
    _putenv("DHD_RENDER=native");
    _putenv("DHD_SCALE=2");
    ID3D11Device *device;
    ID3D11DeviceContext *context;
    if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
                               D3D11_SDK_VERSION, &device, NULL, &context)) ||
        !gs_gpu_init(&gs, device, context) || !gs_gpu_native(&gs)) return 2;

    /* The upload is later than the clear, even though the native draw batch is open. */
    clear(0, 0x80000000);
    gs_write(&gs, GS_BITBLTBUF, 8ULL << 48);
    gs_write(&gs, GS_TRXPOS, 0);
    gs_write(&gs, GS_TRXREG, WIDTH | (uint64_t)HEIGHT << 32);
    gs_write(&gs, GS_TRXDIR, 0);
    for (unsigned i = 0; i < WIDTH * HEIGHT / 2; i++)
        gs_write(&gs, GS_HWREG, 0x800000FF800000FFULL);
    show(0, 0x800000FF, "clear then full-page CPU upload");

    /* A newly drawn source has no older surface owner for the copy to find. */
    clear(128, 0x8000FF00);
    gs_write(&gs, GS_BITBLTBUF, 4096ULL | 8ULL << 16 | 8192ULL << 32 | 8ULL << 48);
    gs_write(&gs, GS_TRXPOS, 0);
    gs_write(&gs, GS_TRXREG, WIDTH | (uint64_t)HEIGHT << 32);
    gs_write(&gs, GS_TRXDIR, 2);
    show(256, 0x8000FF00, "new draw target then local copy");

    clear(128, 0xA500FF00);
    show(128, 0xA500FF00, "colour before high-byte uploads");
    gs_write_pixel(&gs, GS_PSMT4HL, 4096, 8, 7, 9, 3);
    gs_write_pixel(&gs, GS_PSMT4HH, 4096, 8, 8, 9, 4);
    gs_write_pixel(&gs, GS_PSMT8H, 4096, 8, 9, 9, 0x7E);
    gs_write_pixel(&gs, GS_PSMT4HL, 4096, 8, 10, 9, 0);
    show(128, 0xA500FF00, "high-byte uploads preserve every RGB pixel");
    sample_alpha(7, 9, 0xA3, "4HL preserves the high nibble");
    sample_alpha(8, 9, 0x45, "4HH preserves the low nibble");
    sample_alpha(9, 9, 0x7E, "8H replaces the byte");
    sample_alpha(10, 9, 0xA0, "zero nibble is still written");
    sample_alpha(0, 0, 0xA5, "partial uploads preserve other pixels");

    clear(56, 0x8000FF00);
    show(56, 0x8000FF00, "first loading buffer before font upload");
    clear(112, 0x800000FF);
    show(112, 0x800000FF, "second loading buffer before font upload");
    gs_write(&gs, GS_BITBLTBUF, 128ULL << 32 | 8ULL << 48 | (uint64_t)GS_PSMT4HL << 56);
    gs_write(&gs, GS_TRXPOS, 0);
    gs_write(&gs, GS_TRXREG, 512ULL | 512ULL << 32);
    gs_write(&gs, GS_TRXDIR, 0);
    for (unsigned i = 0; i < 512 * 512 / 16; i++) gs_write(&gs, GS_HWREG, 0xFFFFFFFFFFFFFFFFULL);
    show(56, 0x8000FF00, "font alpha preserves the whole first loading buffer");
    show(112, 0x800000FF, "font alpha preserves the second loading buffer's corner");

    gs_gpu_destroy(&gs);
    ID3D11DeviceContext_Release(context);
    ID3D11Device_Release(device);
    return failures ? 1 : 0;
}
