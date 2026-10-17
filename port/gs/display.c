#include "present.h"
#include <string.h>

int gs_display(Gs *g, uint32_t *pixels, unsigned *width, unsigned *height)
{
    uint64_t pmode = gs_priv_read(g, GS_PMODE);
    unsigned circuit = pmode & 1 ? 0 : 1;
    uint64_t fb = gs_priv_read(g, circuit ? GS_DISPFB2 : GS_DISPFB1);
    uint64_t display = gs_priv_read(g, circuit ? GS_DISPLAY2 : GS_DISPLAY1);
    unsigned bp = (unsigned)(fb & 511) * 32, bw = (unsigned)(fb >> 9 & 63);
    unsigned psm = (unsigned)(fb >> 15 & 31), ox = (unsigned)(fb >> 32 & 2047), oy = (unsigned)(fb >> 43 & 2047);
    unsigned w = ((unsigned)(display >> 32 & 4095) + 1) / ((unsigned)(display >> 23 & 15) + 1);
    unsigned h = ((unsigned)(display >> 44 & 2047) + 1) / ((unsigned)(display >> 27 & 3) + 1);
    if (!(pmode & 3) || !bw || !w || !h || w > 1024 || h > 1024) return 0;
    /* INT+FFMD reads a half-height framebuffer (PCSX2 PCRTC). Repeat each
       field line for a stable progressive scanout instead of reading the next buffer. */
    unsigned line_shift = (gs_priv_read(g, GS_SMODE2) & 3) == 3;
    if (g->sync) g->sync(g);
    if (psm == 0 || psm == 1 || psm == 2 || psm == 10) {
        /* Display formats have independent X/Y swizzle bits. Compute each
           axis once instead of decoding the layout again for every pixel. */
        unsigned columns[1024];
        for (unsigned x = 0; x < w; x++) columns[x] = gs_pixel_address(psm, 0, bw, (ox + x) & 2047, 0);
        for (unsigned y = 0; y < h; y++) {
            uint32_t *out = pixels + y * w;
            if (line_shift && (y & 1)) { memcpy(out, out - w, w * 4); continue; }
            unsigned row = gs_pixel_address(psm, bp, bw, 0, (oy + (y >> line_shift)) & 2047);
            for (unsigned x = 0; x < w; x++) {
                unsigned address = (row + columns[x]) & (GS_VRAM_SIZE * 8 - 1);
                uint32_t v = g->vram[address >> 5];
                if (psm & 2) {
                    v >>= address & 31;
                    v = (v & 31) << 3 | (v & 0x3E0) << 6 | (v & 0x7C00) << 9;
                }
                out[x] = v | 0xFF000000;
            }
        }
    } else for (unsigned y = 0; y < h; y++) for (unsigned x = 0; x < w; x++) {
        uint32_t v = gs_read_pixel(g, psm, bp, bw, (ox + x) & 2047, (oy + (y >> line_shift)) & 2047);
        pixels[y * w + x] = gs_color(v, psm, UINT64_C(0x80) << 32) | 0xFF000000;
    }
    *width = w; *height = h;
    return 1;
}
