/* Native renderer (DHD_RENDER=native) on the Rust renderer (port/render, dhd_render.lib): the
   backend half. The shared half is native_core.inc;
   the GPU work (pipelines, arenas, surfaces, textures, conversions, the command loop, scanout and
   the present pass) is Rust's, behind the dhdr_native_* calls of port/render/include/dhd_render.h.
   Included by gpu_rust.c, whose Rust is the exact renderer answering the Z probe.

   What the browser's glue does alike (the device ops, textures, surfaces, the picture as
   native_snapshot.inc's DhdrPicture, the present options, creation's common half) is
   gpu_rust_shared.inc's. What stays here, x64's own:
   - the texture packs: a decoded texture's replacement is its pack's (texpack_view), else its
     sharp font; their memory budget is half the device's dedicated memory (DXGI's
     DedicatedVideoMemory, gpu_rust.c's device.memory), or 1 GB when it is unknown;
   - the window: its size (gpu_rust.c's window_size: GetClientRect, Android: SDL's) for
     DHD_SCALE=window and for each present, pacing (pacing_present,
     not under DHD_INTERP), dhdr_surface_present and the present counts.

   gpu_rust.c's contract with this file (its header): NativeExact is Rust (members x, native,
   clip_on, clip[4], window, surface), native_core.inc's gs_gpu_native and gs_native_* hooks are
   gpu.h's as they are, and this file defines native_init and native_waits_for_vsync. The presented
   test hook (DHD_GS_TEST_BUILD) runs inside dhdr_surface_present. */

typedef Rust NativeExact;

#define NATIVE_GPU_SIDE \
    unsigned scale_limit;             /* the largest scale this card's memory takes (native_scale_limit) */

/* DHD_SCALE up to 8 here (finding 086, S1). native_wanted caps a surface at 2048 GS pixels a side,
   so 8x needs 16384-texel textures: Direct3D 12 guarantees that size
   (D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION) and desktop Vulkan drivers give 16384 or 32768. A size
   the device refuses anyway fails in dhdr_native_surface with a log line, keeping the old texture.
   native_init lowers the scale further on a card with little memory (native_scale_limit). */
#define NATIVE_SCALE_MAX 8u
#include "gpu_rust_shared.inc"   /* native_core.inc, native_snapshot.inc, the shared glue */
#ifdef DHD_GS_TEST_BUILD   /* the GS test programs (replay.c, test_native_*.c) link without present.c */
static void gs_picture_rect(int left, int top, int right, int bottom, int w, int h)
{
    (void)left; (void)top; (void)right; (void)bottom; (void)w; (void)h;
}
#else
void gs_picture_rect(int left, int top, int right, int bottom, int w, int h);  /* present.c: for the mouse */
#endif

static NativeView native_replacement_view(Native *n, NativeTexture *t)
{
    return t->pack ? texpack_view(t->pack, n->field) : t->sharp;
}

/* ---- render thread (context lock held) ---- */

/* DHD_SCALE=window: the smallest scale whose picture is as tall as the window. */
static unsigned native_match_window(Native *n, const NativeFrame *f)
{
    unsigned width, height;
    window_size(n->exact->window, &width, &height);
    unsigned lines = f->display_surface >= 0 && f->display[6] ? f->display[6] : 448, scale = 1;
    while (scale < n->scale_limit && scale * lines < height) scale++;
    fprintf(stderr, "gs native: resolution scale %u (window %u lines, picture %u)\n", scale, height, lines);
    return scale;
}

/* DHD_INTERP shows one picture per display refresh: the surface waits for vsync (Fifo; gpu_rust.c
   asks for DHDR_PRESENT_VSYNC). With DHD_INTERP_HZ its own timer paces the pictures. */
static int native_waits_for_vsync(Native *n)
{
    return n->interp && !n->interp_hz;
}

/* The displayed picture into the window's surface (gpu_rust.c's, made by rust_surface on
   present.c's window), in the rectangle that keeps its shape. */
static void native_present(Native *n)
{
    Rust *p = n->exact;
    if (!p->surface || !p->window || n->present_failed) return;
    unsigned w, h;
    window_size(p->window, &w, &h);
    if (!w || !h) return;  /* minimised */
    static int spikes = -1;
    if (spikes < 0) spikes = getenv("DHD_SPIKES") != NULL;
    double start = spikes ? host_wall_seconds() : 0;
    int drawn = native_present_draw(n, p->surface, w, h, n->resize != 0);
    double ready = spikes ? host_wall_seconds() : 0;
    if (drawn || n->display_width) n->resize = 0;   /* without a picture yet nothing was configured */
    if (drawn <= 0) return;   /* failed, or skipped: no picture yet, timeout */
    {   /* where the picture is in the window, for the mouse (present.c) */
        float viewport[4];
        NativeRect cut;
        native_present_rect(n, w, h, viewport, &cut);
        gs_picture_rect(cut.left, cut.top, cut.right, cut.bottom, (int)w, (int)h);
    }
    if (!n->interp) pacing_present(n->present_due, n->present_began);  /* holds the picture to its due time */
    dhdr_surface_present(p->surface);
    native_note_presented(n);
    if (spikes) {
        double end = host_wall_seconds();
        if (end - start >= 0.001)
            fprintf(stderr, "spike_present: field=%u start=%.9f acquire_ms=%.6f pace_present_ms=%.6f\n",
                    n->field, start, (ready-start)*1e3, (end-ready)*1e3);
    }
}

/* A new surface (gs_gpu_surface_reset; context lock held): presents again, configured afresh. */
static void native_surface_reset(Native *n)
{
    n->present_failed = 0;
    n->resize = 1;
}

/* The displayed picture (display_width x display_height) into pixels; 0 when there is none. */
static int native_read_display(Native *n, uint32_t *pixels)
{
    uint32_t w = 0, h = 0;
    if (!n->display_width || !dhdr_native_read_begin(n->r, &w, &h)) return 0;
    if (w == n->display_width && h == n->display_height) return dhdr_native_read_finish(n->r, pixels);
    /* Not the picture native_picture last reported: finish the read elsewhere, report none. */
    uint32_t *other = malloc((size_t)w * h * 4);
    if (!other) native_oom("display read");
    dhdr_native_read_finish(n->r, other);
    free(other);
    fprintf(stderr, "gs native: display read %ux%u, expected %ux%u\n", w, h, n->display_width, n->display_height);
    return 0;
}

/* ---- creation (EE thread, from rust_init) ---- */

/* The largest scale this card's memory takes: the upscaled surfaces (the frame and Z buffers, about
   NATIVE_SCALED_BYTES at 1x, growing with the scale squared) in at most half its dedicated memory
   (memory bytes, DXGI's DedicatedVideoMemory, as the texture pack's budget); never below 4, as
   before. NATIVE_SCALE_MAX when unknown (0). */
#define NATIVE_SCALED_BYTES (16ull << 20)
static unsigned native_scale_limit(uint64_t memory)
{
    unsigned limit = NATIVE_SCALE_MAX;
    while (memory && limit > 4 && (uint64_t)limit * limit * NATIVE_SCALED_BYTES > memory / 2) limit--;
    return limit;
}

/* 1: the native renderer is installed on g (native_install ran); 0: unavailable, g keeps the exact
   renderer. */
static int native_init(Gs *g, Rust *exact)
{
    double start;
    Native *n = native_create_rust(exact, &start);
    if (!n) return 0;
    /* A texture pack (dhd.ini texture_pack, textures): its default budget is half the card's memory. */
    n->texpack = texpack_open(&n->replace, device.memory);
    n->scale_limit = native_scale_limit(device.memory);
    if (n->scale_setting > n->scale_limit) {
        fprintf(stderr, "gs native: resolution scale %u needs more graphics memory than this card has; %u\n",
                n->scale_setting, n->scale_limit);
        n->scale_setting = n->scale_limit;
    }
    /* What PC SETTINGS > Resolution offers (pc_settings.c reads it; 4 without this renderer). */
    char most[4];
    snprintf(most, sizeof most, "%u", n->scale_limit);
    _putenv_s("DHD_SCALE_MAX", most);
    fprintf(stderr, "gs native: WebGPU shaders and pipelines built in %.2f s\n", host_wall_seconds() - start);
    native_install(g, exact, n, "WebGPU");
    fprintf(stderr, "gs native: GS blending %s\n", n->dual ? "with dual-source blending" :
            "without dual-source blending (a draw blending on As writes its alpha in a second draw)");
    return 1;
}
