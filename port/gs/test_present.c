/* The renderer's window presentation (finding 084, phase W1), headless: a window that is never
   shown, the renderer's surface on its HWND, the scanout picture presented 1:1, stretched 2x and
   shrunk, and a CPU picture (a movie's path). Each presented texture is read back before it is
   presented (gs_wgpu_presented) and checked pixel by pixel against gs_gpu_capture.
   WARP; DHD_GS_TEST_HARDWARE=1: the GPU; DHD_WGPU_BACKEND=vulkan: wgpu on Vulkan instead of
   DX12. Built and run by port/tools/test_gs_wgpu.py. */
#include <windows.h>
#include "gpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const uint32_t *gs_wgpu_presented(unsigned *width, unsigned *height);  /* gpu_rust_test.c */
int test_gpu_init(Gs *g);      /* test_gpu_stubs.c: WARP unless DHD_GS_TEST_HARDWARE=1 */
int test_gpu_hardware(void);

static unsigned failures;
#define CHECK(c) do { if (!(c)) { printf("FAIL present line %d: %s\n", __LINE__, #c); failures++; } } while (0)

static uint32_t picture[1024 * 1024];

/* The window's pixel (x, y) shows the picture's pixel ((2x+1) * w / 2W, ...): nearest, as the
   present pass picks it. */
static unsigned check(const uint32_t *source, unsigned w, unsigned h, unsigned window_w, unsigned window_h)
{
    unsigned width, height, bad = 0;
    const uint32_t *shown = gs_wgpu_presented(&width, &height);
    CHECK(shown && width == window_w && height == window_h);
    if (!shown || width != window_w || height != window_h) return 1;
    for (unsigned y = 0; y < height; y++) for (unsigned x = 0; x < width; x++) {
        unsigned sx = (2 * x + 1) * w / (2 * width), sy = (2 * y + 1) * h / (2 * height);
        uint32_t want = source[sy * w + sx] | 0xFF000000, got = shown[y * width + x];
        if (got != want && bad++ < 4) printf("  (%u,%u): %08X, picture (%u,%u) %08X\n", x, y, got, sx, sy, want);
    }
    return bad;
}

static HWND window;

/* The window's client size (the renderer's GsWindow.size). */
static void client_size(unsigned *width, unsigned *height)
{
    RECT client = {0, 0, 0, 0};
    GetClientRect(window, &client);
    *width = (unsigned)client.right; *height = (unsigned)client.bottom;
}

int main(void)
{
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = DefWindowProcA; wc.hInstance = GetModuleHandleA(NULL); wc.lpszClassName = "dhd_test_present";
    RegisterClassA(&wc);
    /* WS_OVERLAPPEDWINDOW without WS_VISIBLE, and never ShowWindow: nothing appears */
    window = CreateWindowExA(0, wc.lpszClassName, "test_present", WS_OVERLAPPEDWINDOW, 0, 0, 200, 200,
                                  NULL, NULL, wc.hInstance, NULL);
    CHECK(window != NULL);
    gs_reset(&gs);
    if (!test_gpu_init(&gs) || strcmp(gs_gpu_backend_name(&gs), "wgpu")) {
        printf("FAIL the renderer did not start\n"); return 1;
    }
    /* DHD_TEST_EXPECT_SURFACE=0|1 (forwarded by test_gs_wgpu.py) says whether the window gets a
       surface; a surface that exists when 0 is set, or is missing when 1 is set, fails. Unset: the
       surface must work. */
    const char *expect_env = getenv("DHD_TEST_EXPECT_SURFACE");
    int expect_set = expect_env && *expect_env;
    int surface_expected = expect_set ? atoi(expect_env) != 0 : 1;
    GsWindow target = {GS_WINDOW_WIN32, (uintptr_t)window, (uintptr_t)wc.hInstance, client_size};
    int surface = gs_gpu_surface(&gs, &target) != 0;
    if (expect_set && surface != surface_expected) {
        printf("FAIL WebGPU surface %s, DHD_TEST_EXPECT_SURFACE=%d\n", surface ? "exists" : "is missing",
               surface_expected);
        return 1;
    }
    if (!surface) {
        printf("%s no surface on this build and API\n", surface_expected ? "FAIL" : "expected:");
        return surface_expected;
    }
    for (unsigned i = 0; i < GS_VRAM_SIZE / 4; i++) gs.vram[i] = i * 2654435761u;
    gs.pixels_written++;
    memset(gs.host_dirty, 255, sizeof gs.host_dirty);
    gs_priv_write(&gs, GS_PMODE, 2);
    gs_priv_write(&gs, GS_DISPFB2, 0 | UINT64_C(5) << 9); /* CT32, 320 wide, at 0 */
    gs_priv_write(&gs, GS_DISPLAY2, UINT64_C(319) << 32 | UINT64_C(223) << 44);
    unsigned w = 0, h = 0;
    CHECK(gs_gpu_display(&gs, &w, &h) && w == 320 && h == 224);
    CHECK(gs_gpu_capture(&gs, picture));
    static const unsigned sizes[][2] = {{320, 224}, {640, 448}, {1280, 720}, {200, 150}, {320, 224}};
    for (unsigned i = 0; i < sizeof sizes / sizeof *sizes; i++) {
        CHECK(gs_gpu_present(&gs, NULL, 0, 0, sizes[i][0], sizes[i][1]));
        unsigned bad = check(picture, w, h, sizes[i][0], sizes[i][1]);
        printf("scanout %ux%u on a %ux%u window: %u pixels differ\n", w, h, sizes[i][0], sizes[i][1], bad);
        failures += bad != 0;
    }
    /* a CPU picture (movie, DHD_GS_CPU_SCANOUT): 512x256 on 896x672 */
    static uint32_t movie[512 * 256];
    for (unsigned i = 0; i < 512 * 256; i++) movie[i] = (i * 2246822519u) & 0xFFFFFF;
    CHECK(gs_gpu_present(&gs, movie, 512, 256, 896, 672));
    unsigned bad = check(movie, 512, 256, 896, 672);
    printf("CPU picture 512x256 on a 896x672 window: %u pixels differ\n", bad);
    failures += bad != 0;
    CHECK(gs_gpu_present(&gs, NULL, 0, 0, 0, 0)); /* minimised: nothing, no error */
    gs_gpu_destroy(&gs);
    DestroyWindow(window);
    printf("wgpu %s present tests: %u failures\n", test_gpu_hardware() ? "hardware" : "WARP", failures);
    return failures != 0;
}
