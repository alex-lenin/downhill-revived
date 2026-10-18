/* GS pages contain 32 256-byte blocks. These bit permutations implement the
   layouts in PCSX2 GSLocalMemory / GSTables (GPL-3.0+, PCSX2 Dev Team).
   Pixel addresses are in bits, including the high-byte indexed formats. */
#include "gs.h"

/* Conservative physical 8 KB page coverage, including unaligned base pointers
   and VRAM wrap. Z's block XOR changes only bits within a physical page. */
void gs_mark_pages(uint32_t pages[16], unsigned psm, unsigned bp, unsigned bw,
                   unsigned x0, unsigned y0, unsigned x1, unsigned y1)
{
    unsigned pw = psm == 19 || psm == 20 ? 128 : 64;
    unsigned ph = psm == 19 ? 64 : psm == 20 ? 128 : (psm & 7) == 2 ? 64 : 32;
    for (unsigned y = y0 / ph; y <= y1 / ph; y++)
        for (unsigned x = x0 / pw; x <= x1 / pw; x++) {
            unsigned block = bp + (y * (bw * 64 / pw) + x) * 32;
            unsigned first = (block / 32) & 511, last = ((block + 31) / 32) & 511;
            pages[first / 32] |= 1u << (first & 31);
            pages[last / 32] |= 1u << (last & 31);
        }
}

unsigned gs_bits_per_pixel(unsigned psm)
{
    switch (psm) {
    case GS_PSMCT24: case GS_PSMZ24: return 24;
    case GS_PSMCT16: case GS_PSMCT16S: case GS_PSMZ16: case GS_PSMZ16S: return 16;
    case GS_PSMT8: case GS_PSMT8H: return 8;
    case GS_PSMT4: case GS_PSMT4HL: case GS_PSMT4HH: return 4;
    default: return 32;
    }
}

/* Inlined with a constant psm by gs_write_pixels, so its page sizes become shifts. */
static inline __attribute__((always_inline)) uint32_t pixel_address(unsigned psm, unsigned bp, unsigned bw,
                                                                    unsigned x, unsigned y)
{
    unsigned page_w = 64, page_h = 32, block, column, bits = 32, high = 0;
    unsigned bx, by, px, py;
    static const unsigned block16s[8][4] = {
        {0,2,16,18}, {1,3,17,19}, {8,10,24,26}, {9,11,25,27},
        {4,6,20,22}, {5,7,21,23}, {12,14,28,30}, {13,15,29,31}
    };
    if (psm == GS_PSMT8 || psm == GS_PSMT4) {
        page_w = 128;
        page_h = psm == GS_PSMT8 ? 64 : 128;
    } else if ((psm & 7) == 2)
        page_h = 64;
    px = x % page_w;
    py = y % page_h;
    if (psm == GS_PSMT8) {
        bits = 8;
        bx = px / 16; by = py / 16;
        block = (bx & 1) | (by & 1) << 1 | (bx & 2) << 1 | (by & 2) << 2 | (bx & 4) << 2;
        column = (px & 1) << 2 | (px & 2) << 3 | (px & 4) << 3 | (px & 8) >> 2 |
                 (py & 1) << 3 | (py & 2) >> 1 | (py & 4) << 4 | (py & 8) << 4;
        column ^= ((py >> 1 ^ py >> 2) & 1) << 5;
    } else if (psm == GS_PSMT4) {
        bits = 4;
        bx = px / 32; by = py / 16;
        block = (by & 1) | (bx & 1) << 1 | (by & 2) << 1 | (bx & 2) << 2 | (by & 4) << 2;
        column = (px & 1) << 3 | (px & 2) << 4 | (px & 4) << 4 | (px & 8) >> 2 |
                 (px & 16) >> 2 | (py & 1) << 4 | (py & 2) >> 1 | (py & 4) << 5 | (py & 8) << 5;
        column ^= ((py >> 1 ^ py >> 2) & 1) << 6;
    } else if ((psm & 7) == 2) {
        bits = 16;
        bx = px / 16; by = py / 8;
        block = psm & 8 ? block16s[by][bx] :
            (by & 1) | (bx & 1) << 1 | (by & 2) << 1 | (bx & 2) << 2 | (by & 4) << 2;
        column = (px & 1) << 1 | (px & 2) << 2 | (px & 4) << 2 | (px & 8) >> 3 |
                 (py & 1) << 2 | (py & 6) << 4;
    } else {
        bx = px / 8; by = py / 8;
        block = (bx & 1) | (by & 1) << 1 | (bx & 2) << 1 | (by & 2) << 2 | (bx & 4) << 2;
        column = (px & 1) | (px & 6) << 1 | (py & 1) << 1 | (py & 6) << 3;
        if (psm == GS_PSMT8H || psm == GS_PSMT4HL) high = 24;
        if (psm == GS_PSMT4HH) high = 28;
    }
    block += bp + ((y / page_h) * (bw * 64 / page_w) + x / page_w) * 32;
    if ((psm & 0x30) == 0x30)
        block ^= 0x18;  /* Z's XOR applies after adding the base pointer. */
    return (block * 2048 + column * bits + high) & (GS_VRAM_SIZE * 8 - 1);
}

uint32_t gs_pixel_address(unsigned psm, unsigned bp, unsigned bw, unsigned x, unsigned y)
{
    return pixel_address(psm, bp, bw, x, y);
}

uint32_t gs_image_pixel(const uint32_t *vram, unsigned psm, unsigned bp, unsigned bw, unsigned x, unsigned y)
{
    unsigned a = gs_pixel_address(psm, bp, bw, x, y), bits = gs_bits_per_pixel(psm);
    return (vram[a >> 5] >> (a & 31)) & (bits == 32 ? UINT32_MAX : (1u << bits) - 1);
}

uint32_t gs_read_pixel(const Gs *g, unsigned psm, unsigned bp, unsigned bw, unsigned x, unsigned y)
{
    return gs_image_pixel(g->vram, psm, bp, bw, x, y);
}

static inline __attribute__((always_inline)) void write_pixel(Gs *g, unsigned psm, unsigned bp, unsigned bw,
                                                              unsigned x, unsigned y, uint32_t value)
{
    unsigned a = pixel_address(psm, bp, bw, x, y), bits = gs_bits_per_pixel(psm);
    uint32_t mask = bits == 32 ? UINT32_MAX : (1u << bits) - 1;
    unsigned page = a / (8192 * 8), bit = 1u << (page & 31);
    if (!(g->host_dirty[page / 32] & bit) && g->sync_range)
        g->sync_range(g, page * 8192, 8192);
    if ((psm == GS_PSMT8H || psm == GS_PSMT4HL || psm == GS_PSMT4HH) && g->write_high)
        g->write_high(g, a, value, mask);
    g->host_dirty[page / 32] |= bit;
    g->page_writes[page & 511]++;
    uint32_t *word = &g->vram[a >> 5];
    *word = (*word & ~(mask << (a & 31))) | ((value & mask) << (a & 31));
    g->pixels_written++;
}

void gs_write_pixel(Gs *g, unsigned psm, unsigned bp, unsigned bw, unsigned x, unsigned y, uint32_t value)
{
    write_pixel(g, psm, bp, bw, x, y, value);
}

/* Host-to-local transfer of whole pixels, packed as the GIF carries them (4-bit low nibble first,
   24-bit in three bytes): pixel k of the transfer goes to (x0 + k % width, y0 + k / width), wrapped
   at 2048, starting at k = first. The same writes, in the same order, as gs_write_pixel per pixel;
   the format is a constant in each loop so the address arithmetic specializes. */
#define WRITE_PIXELS(PSM, BITS)                                                                          \
    for (unsigned i = 0; i < count; i++) {                                                             \
        uint32_t v = BITS == 32 ? (uint32_t)data[0] | (uint32_t)data[1] << 8 | (uint32_t)data[2] << 16 | \
                                  (uint32_t)data[3] << 24                                              \
                   : BITS == 24 ? (uint32_t)data[0] | (uint32_t)data[1] << 8 | (uint32_t)data[2] << 16 \
                   : BITS == 16 ? (uint32_t)data[0] | (uint32_t)data[1] << 8                           \
                   : BITS == 8 ? data[0] : (i & 1 ? data[0] >> 4 : data[0] & 15u);                     \
        data += BITS == 4 ? (i & 1) : BITS / 8;                                                        \
        write_pixel(g, PSM, bp, bw, x & 2047, y & 2047, v);                                            \
        if (++x == x_end) x = x0, y++;                                                                 \
    }
void gs_write_pixels(Gs *g, unsigned psm, unsigned bp, unsigned bw, unsigned x0, unsigned y0, unsigned width,
                     unsigned first, unsigned count, const uint8_t *data)
{
    unsigned x = x0 + first % width, y = y0 + first / width, x_end = x0 + width;
    switch (psm) {
    case GS_PSMCT32: WRITE_PIXELS(GS_PSMCT32, 32) break;
    case GS_PSMCT24: WRITE_PIXELS(GS_PSMCT24, 24) break;
    case GS_PSMCT16: WRITE_PIXELS(GS_PSMCT16, 16) break;
    case GS_PSMCT16S: WRITE_PIXELS(GS_PSMCT16S, 16) break;
    case GS_PSMT8: WRITE_PIXELS(GS_PSMT8, 8) break;
    case GS_PSMT4: WRITE_PIXELS(GS_PSMT4, 4) break;
    case GS_PSMT8H: WRITE_PIXELS(GS_PSMT8H, 8) break;
    case GS_PSMT4HL: WRITE_PIXELS(GS_PSMT4HL, 4) break;
    case GS_PSMT4HH: WRITE_PIXELS(GS_PSMT4HH, 4) break;
    case GS_PSMZ32: WRITE_PIXELS(GS_PSMZ32, 32) break;
    case GS_PSMZ24: WRITE_PIXELS(GS_PSMZ24, 24) break;
    case GS_PSMZ16: WRITE_PIXELS(GS_PSMZ16, 16) break;
    case GS_PSMZ16S: WRITE_PIXELS(GS_PSMZ16S, 16) break;
    default: {
        unsigned bits = gs_bits_per_pixel(psm);
        if (bits == 32) { WRITE_PIXELS(psm, 32) }
        else if (bits == 24) { WRITE_PIXELS(psm, 24) }
        else if (bits == 16) { WRITE_PIXELS(psm, 16) }
        else if (bits == 8) { WRITE_PIXELS(psm, 8) }
        else { WRITE_PIXELS(psm, 4) }
    }
    }
}
#undef WRITE_PIXELS

uint32_t gs_color(uint32_t value, unsigned psm, uint64_t texa)
{
    if ((psm & 7) == 2) {
        unsigned alpha = value & 0x8000 ? (unsigned)(texa >> 32) & 255 : (unsigned)texa & 255;
        if ((texa & 0x8000) && value == 0) alpha = 0;
        return (value & 31) << 3 | (value & 0x3E0) << 6 | (value & 0x7C00) << 9 | alpha << 24;
    }
    if ((psm & 7) == 1)
        return value | ((texa & 0x8000) && !value ? 0 : (uint32_t)(texa & 255) << 24);
    return value;
}
