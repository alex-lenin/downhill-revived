#include "capture.h"
#include "gpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned failures;
#define CHECK(c) do { if (!(c)) { printf("FAIL capture line %d\n", __LINE__); failures++; } } while (0)
double host_seconds(void) { return 20; }
double host_wall_seconds(void) { return 20; }
uint64_t kernel_vsync_count;
void hle_log(const char *fmt, ...) { (void)fmt; }
int gs_gpu_display(Gs *g, unsigned *w, unsigned *h) { (void)g; (void)w; (void)h; return 0; }
int gs_gpu_capture(Gs *g, uint32_t *pixels) { (void)g; (void)pixels; return 0; }
static uint32_t read32(FILE *f) { uint32_t v = 0; CHECK(fread(&v, 4, 1, f) == 1); return v; }

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 4) return 1;
    unsigned fields = argc >= 3 ? (unsigned)atoi(argv[2]) : 4;
    _putenv_s("DHD_GS_CAPTURE", argv[1]); _putenv_s("DHD_GS_CAPTURE_AT", "0");
    _putenv_s("DHD_GS_CAPTURE_FIELDS", argc >= 3 ? argv[2] : "4");
    if (argc == 4) _putenv_s("DHD_GS_CAPTURE_FRAME", "10");
    gs_reset(&gs);
    gs_priv_write(&gs, GS_PMODE, 1); gs_priv_write(&gs, GS_DISPFB1, 1u << 9);
    gs_priv_write(&gs, GS_DISPLAY1, UINT64_C(3) << 32 | UINT64_C(3) << 44);
    gs_write_pixel(&gs, 0, 0, 1, 2, 1, 0x80765432);
    if (argc == 4) {
        kernel_vsync_count = 9;
        gs_capture_vsync(&gs);
        CHECK(!gs.record_transfer); /* host seconds exceed AT, but field 10 is not due */
        kernel_vsync_count = 10;
    }
    gs_capture_vsync(&gs);
    /* Two fragments become exactly two complete transfer qwords. */
    uint32_t packet[] = {0x8001, 1u << 28, 14, 0, 0x12345678, 0, GS_FOGCOL, 0};
    gs_feed(&gs, 3, packet, 2); gs_feed(&gs, 3, packet + 2, 6);
    for (unsigned i = 1; i < fields; i++) { ++kernel_vsync_count; gs_vblank(&gs); gs_capture_vsync(&gs); }
    CHECK(!gs.record_transfer);
    char name[1024]; snprintf(name, sizeof name, "%s/frame-000.gs", argv[1]);
    FILE *f = fopen(name, "rb"); CHECK(f != NULL); if (!f) return 1;
    CHECK(read32(f) == 0);
    uint32_t state_size = read32(f); CHECK(state_size == 4194752);
    CHECK(read32(f) == 8);
    /* Initial state, privileged image and the first VSync. */
    fseek(f, 8 + state_size + 8192, SEEK_SET);
    CHECK(fgetc(f) == 3); fseek(f, 8192, SEEK_CUR);
    CHECK(fgetc(f) == 1); CHECK(fgetc(f) == 0);
    CHECK(fgetc(f) == 0 && fgetc(f) == 3 && read32(f) == 32);
    uint32_t words[8]; CHECK(fread(words, 32, 1, f) == 1);
    CHECK(memcmp(words, packet, 32) == 0);
    for (unsigned i = 0; i < fields-1; i++) {
        CHECK(fgetc(f) == 3); fseek(f, 8192, SEEK_CUR);
        CHECK(fgetc(f) == 1); CHECK(fgetc(f) == (int)((i + 1) & 1));
    }
    CHECK(fgetc(f) == EOF); fclose(f);
    printf("PCSX2 GS dump serialization and fragmented transfers: %u failures\n", failures);
    return failures != 0;
}
