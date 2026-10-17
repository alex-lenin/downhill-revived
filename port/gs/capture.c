/* PCSX2 version-8 freeze/dump wire format, written independently from its
   documented serializers. No upstream implementation copied. */
#include "capture.h"
#include "present.h"
#include "gpu.h"
#include "../hle/hle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *dump;
static volatile char requested[64];  /* gs_capture_request's name, until the capture starts */
static unsigned capture, frames, length;  /* fields in the capture being written */
static char prefix[1024];
static uint32_t pixels[1024 * 1024];
static uint32_t packet[65536];
static unsigned packet_path, packet_words;
static void bytes(const void *data, size_t size) { fwrite(data, 1, size, dump); }
static void u32(uint32_t value) { bytes(&value, 4); }
static void u64(uint64_t value) { bytes(&value, 8); }
static void u8(unsigned value) { unsigned char v = (unsigned char)value; bytes(&v, 1); }

static void flush_transfer(void)
{
    if (!packet_words) return;
    u8(0); u8(packet_path); u32(packet_words * 4); bytes(packet, packet_words * 4);
    packet_words = 0;
}

static void transfer(unsigned path, const uint32_t *qword)
{
    if (path != packet_path || packet_words == sizeof packet / sizeof *packet) flush_transfer();
    packet_path = path;
    memcpy(packet + packet_words, qword, 16); packet_words += 4;
}

static void registers(Gs *g)
{
    uint8_t priv[8192] = {0};
    /* Privileged registers occupy the first half of each 16-byte slot. */
    for (unsigned i = 0; i < sizeof g->privileged / sizeof *g->privileged; i++)
        memcpy(priv + i * 16, &g->privileged[i], 8);
    bytes(priv, sizeof priv);
}

static void initial_state(Gs *g)
{
    static const unsigned env[] = {GS_PRIM, GS_PRMODECONT, GS_TEXCLUT, GS_SCANMSK,
        GS_TEXA, GS_FOGCOL, GS_DIMX, GS_DTHE, GS_COLCLAMP, GS_PABE, GS_BITBLTBUF,
        GS_TRXDIR, GS_TRXPOS, GS_TRXREG, GS_TRXREG};
    static const unsigned context[] = {GS_XYOFFSET_1, GS_TEX0_1, GS_TEX1_1,
        GS_CLAMP_1, 52, 54, GS_SCISSOR_1, GS_ALPHA_1, GS_TEST_1, GS_FBA_1,
        GS_FRAME_1, GS_ZBUF_1};
    const uint32_t state_size = 4 + 15 * 8 + 2 * 12 * 8 + 4 * 8 + 2 * 4 + 8 + GS_VRAM_SIZE + 4 * 20 + 4;
    u32(0); u32(state_size); /* legacy dump header: CRC, freeze size */
    u32(8);
    for (unsigned i = 0; i < sizeof env / sizeof *env; i++) u64(g->reg[env[i]]);
    for (unsigned c = 0; c < 2; c++) for (unsigned i = 0; i < sizeof context / sizeof *context; i++) {
        uint64_t value = g->reg[context[i] + c];
        if (context[i] == GS_ZBUF_1) value |= UINT64_C(48) << 24;
        u64(value);
    }
    uint32_t q; memcpy(&q, &g->vertex.q, 4);
    u64(g->vertex.rgba | (uint64_t)q << 32);
    bytes(&g->vertex.s, 8);
    u32(g->vertex.u | g->vertex.v << 16);
    u32(g->vertex.fog);
    u64(g->vertex.x | (uint64_t)g->vertex.y << 16 | (uint64_t)g->vertex.z << 32);
    u64(0); /* obsolete register */
    u32(0); u32(0); /* no partial host transfer at the capture boundary */
    bytes(g->vram, GS_VRAM_SIZE);
    for (unsigned i = 0; i < 4; i++) {
        if (i < 3) {
            const GsPath *p = &g->path[i];
            u64(p->left | (uint64_t)p->eop << 15 | (uint64_t)p->mode << 58 | (uint64_t)(p->nreg & 15) << 60);
            u64(p->regs); u32(p->reg);
        } else { u64(0); u64(0); u32(0); }
    }
    u32(q);
    registers(g);
}

static void image(Gs *g)
{
    unsigned w = 0, h = 0;
    if (!gs_gpu_display(g, &w, &h) || !gs_gpu_capture(g, pixels))
        if (!gs_display(g, pixels, &w, &h)) return;
    char path[1100]; snprintf(path, sizeof path, "%s-native-%03u.bmp", prefix, frames);
    FILE *f = fopen(path, "wb");
    if (!f) return;
    uint8_t header[54] = {0x42, 0x4d};
    uint32_t size = 54 + w * h * 4, offset = 54, dib = 40, depth = 32;
    int32_t height = -(int32_t)h;
    memcpy(header + 2, &size, 4); memcpy(header + 10, &offset, 4);
    memcpy(header + 14, &dib, 4); memcpy(header + 18, &w, 4); memcpy(header + 22, &height, 4);
    header[26] = 1; memcpy(header + 28, &depth, 2);
    fwrite(header, 1, sizeof header, f);
    for (unsigned i = 0; i < w * h; i++) {
        uint32_t p = pixels[i], bgra = (p & 0xFF00FF00) | (p & 255) << 16 | (p >> 16 & 255);
        fwrite(&bgra, 4, 1, f);
    }
    fclose(f);
}

void gs_capture_request(const char *name)
{
    if (!getenv("DHD_RACE_GSDUMP") || requested[0]) return;
    snprintf((char *)requested, sizeof requested, "%s", name);
}

void gs_capture_vsync(Gs *g)
{
    static int initialized;
    static const char *directory;
    static double next, interval;
    static unsigned fields = 4;
    if (!initialized) {
        initialized = 1; directory = getenv("DHD_GS_CAPTURE");
        const char *at = getenv("DHD_GS_CAPTURE_AT"), *every = getenv("DHD_GS_CAPTURE_INTERVAL");
        if (getenv("DHD_GS_CAPTURE_FRAME")) {
            at = getenv("DHD_GS_CAPTURE_FRAME");
            every = getenv("DHD_GS_CAPTURE_FRAME_INTERVAL");
        }
        next = at ? atof(at) : 10; interval = every ? atof(every) : 0;
        const char *length = getenv("DHD_GS_CAPTURE_FIELDS");
        if (length) fields = strtoul(length, NULL, 0);
        if (!fields || fields > 600) fields = 4;
    }
    if (!directory && !requested[0] && !dump) return;
    if (!dump) {
        extern uint64_t kernel_vsync_count;
        if (!requested[0]) {
            if (capture && interval <= 0) return;
            double capture_time = getenv("DHD_GS_CAPTURE_FRAME") ? (double)kernel_vsync_count : host_seconds();
            if (capture_time < next) return;
        }
        /* Freeze has no fragment buffer or unfinished host-image transfer. */
        for (unsigned i = 0; i < 3; i++) if (g->path[i].have || g->path[i].left) return;
        if (g->sync) g->sync(g);
        length = requested[0] ? 2 : fields;
        if (requested[0]) {
            snprintf(prefix, sizeof prefix, "%s/gs-%s", getenv("DHD_RACE_GSDUMP"), (const char *)requested);
            requested[0] = 0;
        } else
            snprintf(prefix, sizeof prefix, "%s/frame-%03u", directory, capture++);
        char path[1100]; snprintf(path, sizeof path, "%s.gs", prefix);
        dump = fopen(path, "wb");
        if (!dump) return;
        setvbuf(dump, NULL, _IOFBF, 1024 * 1024);
        initial_state(g); frames = 0;
        g->record_transfer = transfer;
    }
    flush_transfer();
    u8(3); registers(g); u8(1); u8((unsigned)(gs_priv_read(g, GS_CSR) >> 13) & 1);
    image(g);
    extern uint64_t kernel_vsync_count;
    fprintf(stderr, "compare: dump %s field=%u vsync=%llu host=%.6f game=%.6f\n", prefix, frames,
            (unsigned long long)kernel_vsync_count, host_wall_seconds(), host_seconds());
    if (++frames == length) {
        fclose(dump); dump = NULL; g->record_transfer = NULL;
        if (directory) next += interval;
        hle_log("gs: captured %s.gs and %u matching native fields\n", prefix, length);
    }
}
