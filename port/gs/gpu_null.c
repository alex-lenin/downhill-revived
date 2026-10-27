/* The GPU backend where there is no graphics device (headless Linux): every entry point keeps the
   no-renderer semantics (gs_gpu_display/capture return 0, the native renderer reports absent).
   Nothing here changes game state: draws, syncs and copies stay unset, so the software GS memory
   paths answer every read. Interface: port/gs/gpu.h. */
#include "gpu.h"

#include <stddef.h>

void gs_gpu_destroy(Gs *g) { (void)g; }

void gs_gpu_profile(Gs *g, double seconds)
{
    (void)g;
    (void)seconds;
}

int gs_gpu_display(Gs *g, unsigned *width, unsigned *height)
{
    (void)g;
    (void)width;
    (void)height;
    return 0; /* no backend: the display-circuit decoder answers (display.c) */
}

int gs_gpu_capture(Gs *g, uint32_t *pixels)
{
    (void)g;
    (void)pixels;
    return 0;
}

const char *gs_gpu_backend_name(const Gs *g)
{
    (void)g;
    return "none";
}

int gs_gpu_surface(Gs *g, void *window)
{
    (void)g;
    (void)window;
    return 0;
}

int gs_gpu_present(Gs *g, const uint32_t *pixels, unsigned width, unsigned height,
                   unsigned window_width, unsigned window_height)
{
    (void)g; (void)pixels; (void)width; (void)height; (void)window_width; (void)window_height;
    return 0;
}

int gs_gpu_init(Gs *g)
{
    (void)g;
    return 0; /* no device on this host */
}

int gs_gpu_native(const Gs *g)
{
    (void)g;
    return 0;
}

void gs_native_vsync(Gs *g, unsigned *width, unsigned *height, int resize)
{
    (void)g;
    (void)width;
    (void)height;
    (void)resize;
}

void gs_native_vsync_handed(Gs *g, unsigned *width, unsigned *height, int resize, double due, double began,
                            int flat_limit)
{
    (void)due, (void)began, (void)flat_limit;
    gs_native_vsync(g, width, height, resize);
}

void gs_native_bench(Gs *g, int end)
{
    (void)g;
    (void)end;
}

void gs_native_lock(Gs *g, int lock)
{
    (void)g;
    (void)lock;
}

void gs_native_present_options(Gs *g) { (void)g; }

uint32_t *gs_native_picture(Gs *g, unsigned *width, unsigned *height)
{
    (void)g;
    (void)width;
    (void)height;
    return NULL;
}

void gs_native_movie(Gs *g, const uint32_t *rgba, unsigned width, unsigned height)
{
    (void)g;
    (void)rgba;
    (void)width;
    (void)height;
}
