/* The depth-plane harness for the renderer (finding 084, W4). Planes are made as the exact
   renderer's draw makes them; each case is one pixel's Z, worked out three ways:
     cpu    IEEE doubles here (SSE2, no contraction): the definition
     exact  raster.wgsl's software binary64 (what the renderer uses)
     pair   raster.wgsl's double-float (two f32) evaluation, finding 084's first proposal
   and every result is compared with cpu (the retired D3D11 renderer's precise doubles matched cpu
   on every case). usage: test_depth [cases-per-kind] (default 1M).
   DHD_GS_TEST_HARDWARE=1 runs on the GPU, else on WARP. DHD_TEST_EE_MXCSR=1 starts the renderer
   (device and depth_probe pipeline) under the EE thread's MXCSR. */
#include <windows.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gs_wgpu_depth_probe(const void *cases, unsigned count, uint32_t *results);
const char *gs_wgpu_adapter(void);

typedef struct { uint32_t depth[8]; int32_t offset[4]; float pairs[8]; } Case;

static uint64_t state = 0x9E3779B97F4A7C15ull;
static uint32_t next(void)
{
    state ^= state << 13; state ^= state >> 7; state ^= state << 17;
    return (uint32_t)(state >> 16);
}
static int range(int lo, int hi) { return lo + (int)(next() % (uint32_t)(hi - lo + 1)); }

static uint32_t depth_integer(double z)
{
    if (z <= 0.0) return 0;
    if (z >= 4294967296.0) return 0xFFFFFFFF;  /* the D3D11 shader's 4294967295.0 was a float literal */
    return (uint32_t)floor(z);
}
static uint32_t cpu_depth(const double d[3], int dx, int dy)
{
    double depth = d[0];
    depth += d[1] * (double)(float)dx;
    depth += d[2] * (double)(float)dy;
    return depth_integer(depth);
}

/* The exact renderer's draw: x, y in 12.4 window coordinates, z the vertex Z. */
static void plane(const int x[3], const int y[3], const uint32_t z[3], int line, double d[3])
{
    d[0] = z[0]; d[1] = d[2] = 0;
    if (line) {
        int dx = x[1] - x[0], dy = y[1] - y[0];
        unsigned axis = abs(dx) >= abs(dy) ? 1 : 2;
        int distance = axis == 1 ? dx : dy;
        if (distance) d[axis] = ((double)z[1] - z[0]) / distance;
        return;
    }
    double dx1 = x[1] - x[0], dy1 = y[1] - y[0], dx2 = x[2] - x[0], dy2 = y[2] - y[0];
    double dz1 = (double)z[1] - z[0], dz2 = (double)z[2] - z[0];
    double area = dx1 * dy2 - dy1 * dx2;
    if (area) { d[1] = (dz1 * dy2 - dy1 * dz2) / area; d[2] = (dx1 * dz2 - dz1 * dx2) / area; }
}

static void make_case(Case *c, const double d[3], int dx, int dy)
{
    memcpy(c->depth, d, 3 * sizeof *d);
    c->depth[6] = c->depth[7] = 0;
    c->offset[0] = dx; c->offset[1] = dy; c->offset[2] = c->offset[3] = 0;
    for (unsigned i = 0; i < 3; i++) {
        float hi = (float)d[i];
        c->pairs[i * 2] = hi; c->pairs[i * 2 + 1] = (float)(d[i] - hi);
    }
    c->pairs[6] = c->pairs[7] = 0;
}

/* The kinds of plane: what the game draws and what is hard for the arithmetic. */
static const char *kinds[] = {"triangles", "small", "lines", "far24", "quarters"};
enum { KINDS = 5 };
static void generate(unsigned kind, Case *c, double d[3])
{
    int x[3], y[3]; uint32_t z[3];
    unsigned zbits = next() % 3;  /* 32-, 24- or 16-bit Z */
    uint32_t zmask = zbits == 0 ? 0xFFFFFFFF : zbits == 1 ? 0xFFFFFF : 0xFFFF;
    for (;;) {
        if (kind == 0) {        /* random screen triangles */
            for (int i = 0; i < 3; i++) { x[i] = range(0, 640 * 16); y[i] = range(0, 512 * 16); z[i] = next() << 16 ^ next(); z[i] &= zmask; }
        } else if (kind == 1) { /* small triangles with small Z steps: non-dyadic ratios near integers */
            x[0] = range(0, 600 * 16); y[0] = range(0, 480 * 16); z[0] = (next() << 16 ^ next()) & zmask;
            for (int i = 1; i < 3; i++) {
                x[i] = x[0] + range(-48 * 16, 48 * 16); y[i] = y[0] + range(-48 * 16, 48 * 16);
                z[i] = z[0] + (uint32_t)range(-9, 9);
            }
        } else if (kind == 2) { /* lines */
            for (int i = 0; i < 2; i++) { x[i] = range(0, 640 * 16); y[i] = range(0, 512 * 16); z[i] = (next() << 16 ^ next()) & zmask; }
            x[2] = y[2] = 0; z[2] = 0;
        } else if (kind == 3) { /* the far end of a 24-bit buffer: large Z, tiny slopes */
            for (int i = 0; i < 3; i++) { x[i] = range(0, 640 * 16); y[i] = range(0, 512 * 16); z[i] = 0xFFFF00u + (uint32_t)range(0, 255); }
        } else {                /* whole pixels, Z steps of whole units: exact quarters and thirds */
            int s = range(1, 64) * 16;
            x[0] = range(0, 500) * 16; y[0] = range(0, 400) * 16; z[0] = (next() << 16 ^ next()) & zmask;
            x[1] = x[0] + s; y[1] = y[0]; x[2] = x[0]; y[2] = y[0] + range(1, 64) * 16;
            z[1] = z[0] + (uint32_t)range(0, 12); z[2] = z[0] + (uint32_t)range(0, 12);
        }
        plane(x, y, z, kind == 2, d);
        if (d[1] != 0 || d[2] != 0) break;
    }
    /* A pixel in the primitive's box, as draw_pixel's p - a (12.4 units, pixel corners). */
    int minx = x[0], maxx = x[0], miny = y[0], maxy = y[0];
    for (int i = 1; i < (kind == 2 ? 2 : 3); i++) {
        if (x[i] < minx) minx = x[i]; if (x[i] > maxx) maxx = x[i];
        if (y[i] < miny) miny = y[i]; if (y[i] > maxy) maxy = y[i];
    }
    int px = range(minx >> 4, (maxx + 15) >> 4) * 16, py = range(miny >> 4, (maxy + 15) >> 4) * 16;
    make_case(c, d, px - x[0], py - y[0]);
}

unsigned test_ee_mxcsr_enter(void);          /* test_gpu_stubs.c */
void test_ee_mxcsr_leave(unsigned csr);

/* The renderer's side. Its first call starts the backend's device and compiles depth_probe: that start
   runs under the EE mode (DHD_TEST_EE_MXCSR=1, test_gpu_stubs.c), as gs_gpu_init does in the other
   test programs; every later call runs in the caller's mode. */
static int wgpu_probe(const Case *cases, unsigned count, uint32_t *results)
{
    static int started;
    if (started) return gs_wgpu_depth_probe(cases, count, results);
    started = 1;
    unsigned csr = test_ee_mxcsr_enter();
    int ok = gs_wgpu_depth_probe(cases, count, results);
    test_ee_mxcsr_leave(csr);
    return ok;
}

int main(int argc, char **argv)
{
    unsigned per_kind = argc > 1 ? (unsigned)strtoul(argv[1], NULL, 0) : 1u << 20, chunk = 1u << 18;
    const char *hardware_option = getenv("DHD_GS_TEST_HARDWARE");
    int hardware = hardware_option && atoi(hardware_option);
    if (!getenv("DHD_WGPU_ADAPTER")) _putenv_s("DHD_WGPU_ADAPTER", hardware ? "hardware" : "fallback");
    Case *cases = malloc(chunk * sizeof *cases);
    uint32_t *wgpu_z = malloc(chunk * 8), *cpu_z = malloc(chunk * 4);
    unsigned failures = 0;
    for (unsigned kind = 0; kind < KINDS; kind++) {
        unsigned long long exact_bad = 0, pair_bad = 0, total = 0;
        int shown = 0;
        for (unsigned done = 0; done < per_kind; done += chunk) {
            unsigned count = per_kind - done < chunk ? per_kind - done : chunk;
            for (unsigned i = 0; i < count; i++) {
                double d[3];
                generate(kind, &cases[i], d);
                cpu_z[i] = cpu_depth(d, cases[i].offset[0], cases[i].offset[1]);
            }
            if (!wgpu_probe(cases, count, wgpu_z)) {
                printf("FAIL probe dispatch\n"); return 1;
            }
            for (unsigned i = 0; i < count; i++) {
                exact_bad += wgpu_z[i * 2] != cpu_z[i];
                if (wgpu_z[i * 2 + 1] != cpu_z[i]) {
                    pair_bad++;
                    if (shown++ < 2) {
                        double d[3]; memcpy(d, cases[i].depth, sizeof d);
                        printf("  pair differs: d0=%.17g dzdx=%.17g dzdy=%.17g dx=%d dy=%d cpu=%u pair=%u\n",
                               d[0], d[1], d[2], cases[i].offset[0], cases[i].offset[1], cpu_z[i], wgpu_z[i * 2 + 1]);
                    }
                }
            }
            total += count;
        }
        printf("%-10s cases=%llu exact_vs_cpu=%llu pair_vs_cpu=%llu\n",
               kinds[kind], total, exact_bad, pair_bad);
        failures += exact_bad != 0;
    }
    printf("depth probe on %s / %s: %s\n", hardware ? "hardware" : "WARP", gs_wgpu_adapter(),
           failures ? "FAIL" : "exact matches everywhere");
    return failures != 0;
}
