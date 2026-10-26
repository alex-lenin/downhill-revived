/* The browser's graphics queue (web_queue.h), both sides:
   - WEB_QUEUE_PRODUCER: the core's side, build/web/gqueue.wasm (build_graphics.py --threads, with
     its generated call wrappers gqueue_calls.c). The core's graphics imports link to its web_x_*
     exports through www/thunks.js, as they link to the reactor's in the one-worker build.
   - otherwise: the reactor's side, linked into graphics.wasm (build_graphics.py --threads): the
     ring and web_q_run, which www/graphics.js (or headless_graphics.mjs) calls in a loop.
   Positions only grow (32-bit wrap); each side writes its own and reads the other's with
   sequentially consistent atomics. A side that sleeps says so first (sleeping, waiting) and reads
   the other's position again, so the other side's notify after its publication is never lost (the
   handshake of port/hle/graphics_queue.c's idle_wait, on wasm's wait and notify). */
#include "web_queue.h"

static inline uint32_t load(const uint32_t *p) { return __atomic_load_n(p, __ATOMIC_SEQ_CST); }
static inline void store(uint32_t *p, uint32_t v) { __atomic_store_n(p, v, __ATOMIC_SEQ_CST); }
static inline WebPacket *packet(uint32_t at)
{
    return (WebPacket *)(uintptr_t)(WQ->ring + (at & (WEB_Q_RING_BYTES - 1)));
}

#ifdef WEB_QUEUE_PRODUCER
/* ---- the core's side ---- */

/* The core's FP control word (the float helpers postlink.py put into the core read it here; the
   page's fiber switches save and restore it), as host_graphics.c has it in the one-worker build. */
#define WEB_CSR (*(volatile uint32_t *)(uintptr_t)0x07800000u)
__attribute__((export_name("web_x_host_fp_csr"))) unsigned web_x_host_fp_csr(void) { return WEB_CSR; }
__attribute__((export_name("web_x_host_fp_set_csr"))) void web_x_host_fp_set_csr(unsigned csr) { WEB_CSR = csr; }
__attribute__((export_name("web_x_host_fp_mode"))) void web_x_host_fp_mode(int mode) { WEB_CSR = mode ? 0xffc0u : 0x1f80u; }

/* build_graphics.py's guest address fold (the reactor's web_x_* wrappers fold the same way). */
static inline uint32_t web_fold(uint32_t a)
{
    uint32_t low = a & 0x1fffffffu;
    if (a >= 0x38000000u && a < 0x3a000000u) return a - 0x30000000u;
    if (a >= 0x30400000u && a < 0x32000000u) return a - 0x28000000u;
    if ((a & 0xe0000000u) == 0 || (a & 0xe0000000u) == 0x20000000u) {
        if (low >= 0x08000000u && low < 0x0a000000u) return low;
        if (low >= 0x00400000u && low < 0x02000000u) return low + 0x08000000u;
    }
    return a;
}

/* Until consumed moves away from seen: a short spin, then wasm's wait (a dedicated worker may
   block). The graphics worker sets failed when it stops; the core then traps instead of hanging. */
static void wait_consumed(uint32_t seen)
{
    WebQueue *q = WQ;
    for (unsigned spin = 0; spin < 4096; spin++)
        if (load(&q->consumed) != seen) return;
    __atomic_fetch_add(&q->waiting, 1, __ATOMIC_SEQ_CST);
    while (load(&q->consumed) == seen) {
        if (load(&q->failed)) __builtin_trap();
        __builtin_wasm_memory_atomic_wait32((int *)&q->consumed, (int)seen, 100000000LL);
    }
    __atomic_fetch_sub(&q->waiting, 1, __ATOMIC_SEQ_CST);
}

/* Until the reactor has retired everything up to end. */
static void wait_until(uint32_t end)
{
    for (;;) {
        uint32_t done = load(&WQ->consumed);
        if ((int32_t)(done - end) >= 0) return;
        wait_consumed(done);
    }
}

/* Room for one packet of bytes at the head (a pad packet first when it would cross the ring's
   end): its position. Only this side writes written. */
static uint32_t reserve(uint32_t bytes)
{
    WebQueue *q = WQ;
    if (!q->ring) __builtin_trap();   /* the reactor's web_q_init has not run */
    uint32_t at = q->written;
    uint32_t tail = WEB_Q_RING_BYTES - (at & (WEB_Q_RING_BYTES - 1)), wrap = bytes > tail ? tail : 0;
    for (;;) {
        uint32_t done = load(&q->consumed);
        if (WEB_Q_RING_BYTES - (at - done) >= bytes + wrap) break;
        wait_consumed(done);
    }
    if (wrap) {
        WebPacket *pad = packet(at);
        pad->bytes = wrap;
        pad->kind = WQ_PAD;
        at += wrap;
    }
    return at;
}

static void publish(uint32_t end)
{
    WebQueue *q = WQ;
    store(&q->written, end);
    if (load(&q->sleeping)) __builtin_wasm_memory_atomic_notify((int *)&q->written, 1);
}

/* host_graphics.c's web_vu_ready, decided where the one-worker build decides it: the VU1 image
   (the core loads it into guest RAM) is there when the call is made, not when the reactor runs it. */
static uint32_t vu_ready(void)
{
    WebQueue *q = WQ;
    if (!q->vu_seen) {
        const volatile uint32_t *image = (const volatile uint32_t *)(uintptr_t)(0x08000000u + 0x263BC0u);
        for (unsigned i = 0; i < 1024; i++)
            if (image[i]) { q->vu_seen = 1; break; }
    }
    return q->vu_seen ? WQ_VU_READY : 0;
}

/* VIF1/GIF data, copied now (the core may change it once this returns). A transfer larger than a
   quarter of the ring goes in parts, as graphics_queue.c's (a DMA block is at most 1 MiB). */
static void feed(uint32_t kind, int unit, uint32_t address, uint32_t words)
{
    const uint32_t *from = (const uint32_t *)(uintptr_t)web_fold(address);
    uint32_t flags = vu_ready();
    do {
        uint32_t take = words > WEB_Q_RING_BYTES / 16 ? WEB_Q_RING_BYTES / 16 : words;
        uint32_t bytes = (uint32_t)(sizeof(WebPacket) + take * 4 + 15) & ~15u;
        uint32_t at = reserve(bytes);
        WebPacket *p = packet(at);
        p->bytes = bytes;
        p->kind = kind | flags;
        p->a = (uint32_t)unit;
        p->b = take;
        __builtin_memcpy(p + 1, from, take * 4);
        publish(at + bytes);
        from += take;
        words -= take;
    } while (words);
}

__attribute__((export_name("web_x_host_service_graphics_feed")))
void web_x_host_service_graphics_feed(int unit, uint32_t address, uint32_t words) { feed(WQ_GRAPHICS, unit, address, words); }
__attribute__((export_name("web_x_host_service_vif_feed")))
void web_x_host_service_vif_feed(int unit, uint32_t address, uint32_t words) { feed(WQ_VIF, unit, address, words); }
/* Every place the native EE waits for its graphics workers. */
__attribute__((export_name("web_x_host_service_graphics_wait")))
void web_x_host_service_graphics_wait(void) { wait_until(WQ->written); }

/* A GS/VU1 call (gqueue_calls.c's wrappers): queued; with wait, the core waits for its result. */
uint64_t web_q_call(uint32_t id, uint32_t wait, uint32_t flags, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint32_t at = reserve(48);
    WebPacket *p = packet(at);
    p->bytes = 48;
    p->kind = WQ_CALL | flags;
    p->a = id;
    p->b = wait;
    uint64_t *args = (uint64_t *)(p + 1);
    args[0] = a0; args[1] = a1; args[2] = a2;
    publish(at + 48);
    if (!wait) return 0;
    wait_until(at + 48);
    return WQ->result;
}
uint32_t web_q_vu_ready(void) { return vu_ready(); }

/* worker.js's host_frame_context: the reactor's kernel_vsync_count (web_frame), in order. */
__attribute__((export_name("web_q_frame"))) void web_q_frame(uint64_t field)
{
    uint32_t at = reserve(32);
    WebPacket *p = packet(at);
    p->bytes = 32;
    p->kind = WQ_FRAME;
    p->a = p->b = 0;
    *(uint64_t *)(p + 1) = field;
    publish(at + 32);
}

/* worker.js's host_service_gs_vsync: web_display with the GS register pages as they are now. */
__attribute__((export_name("web_q_vsync"))) void web_q_vsync(void)
{
    uint32_t at = reserve(144);
    WebPacket *p = packet(at);
    p->bytes = 144;
    p->kind = WQ_VSYNC;
    p->a = p->b = 0;
    uint64_t *regs = (uint64_t *)(p + 1);
    for (unsigned i = 0; i < 15; i++) regs[i] = *(volatile uint64_t *)(uintptr_t)(0x12000000u + 16 * i);
    publish(at + 144);
}

/* worker.js's host_putenv: "NAME=value" (bytes long, at text) for the reactor's environment. */
__attribute__((export_name("web_q_env"))) void web_q_env(uint32_t text, uint32_t bytes)
{
    if (bytes > 4000) __builtin_trap();
    uint32_t size = (uint32_t)(sizeof(WebPacket) + bytes + 15) & ~15u;
    uint32_t at = reserve(size);
    WebPacket *p = packet(at);
    p->bytes = size;
    p->kind = WQ_ENV;
    p->a = bytes;
    p->b = 0;
    __builtin_memcpy(p + 1, (const void *)(uintptr_t)text, bytes);
    publish(at + size);
}

#else
/* ---- the reactor's side ---- */
#include "../hle/gs_queue.h"   /* graphics_gif_feed */
void vif_feed(int unit, const uint32_t *words, uint32_t count);
void web_vu_ready(void);
void web_graphics_enter(void);
void web_frame(uint64_t field);
uint32_t web_display(void);
int web_present(uint32_t canvas_w, uint32_t canvas_h);
void host_fp_set_csr(unsigned csr);
extern const uint64_t *web_gs_mmio_snapshot;
uint64_t web_q_dispatch(uint32_t id, const uint64_t *args);   /* build_graphics.py's web_exports.c */

static uint8_t web_ring[WEB_Q_RING_BYTES] __attribute__((aligned(64)));
static char env_copy[4096];

/* Once, before the core starts: the ring's address for the core's side. The reactor's own FP
   control word (WEB_Q_REACTOR_CSR) is set to the GS front end's IEEE mode here and stays in it
   between calls (VU1's VU mode under --exact-float and graphics_gif_feed switch it back), so no
   packet switches it, unlike the one-worker build's web_x_* wrappers on the core's word. */
__attribute__((export_name("web_q_init"))) uint32_t web_q_init(void)
{
    host_fp_set_csr(0x1f80);
    store(&WQ->ring, (uint32_t)(uintptr_t)web_ring);
    return WEB_Q_BLOCK;
}

/* Runs packets until the ring is empty (WQ_RUN_EMPTY: the JS loop waits on written, sleeping is
   set), a vsync was drawn (WQ_RUN_PRESENT: display is web_display's answer; the loop shows it and
   ends its task) or the environment changed (WQ_RUN_ENV: env_text, env_bytes). Called through JSPI's
   promising: a readback inside suspends it. */
__attribute__((export_name("web_q_run"))) int web_q_run(void)
{
    WebQueue *q = WQ;
    store(&q->sleeping, 0);
    uint32_t at = load(&q->consumed);
    for (;;) {
        if (load(&q->written) == at) {
            for (unsigned spin = 0; spin < 32768 && load(&q->written) == at; spin++) {}
            if (load(&q->written) == at) {
                store(&q->sleeping, 1);
                if (load(&q->written) == at) return WQ_RUN_EMPTY;
                store(&q->sleeping, 0);
            }
            continue;
        }
        WebPacket *p = packet(at);
        uint32_t kind = p->kind & WQ_KIND, bytes = p->bytes;
        int answer = -1;
        if (kind == WQ_CALL) {
            /* the web_x_* wrapper folds the addresses and checks the VU1 image */
            uint64_t r = web_q_dispatch(p->a, (const uint64_t *)(p + 1));
            if (p->b) q->result = r;
        } else if (kind != WQ_PAD) {
            /* what build_graphics.py's web_x_* wrappers do around each call */
            web_graphics_enter();
            if (p->kind & WQ_VU_READY) web_vu_ready();
            const uint32_t *words = (const uint32_t *)(p + 1);
            const uint64_t *args = (const uint64_t *)(p + 1);
            switch (kind) {
            case WQ_VIF:
                vif_feed((int)p->a, words, p->b);
                break;
            case WQ_GRAPHICS:   /* host_graphics.c's host_service_graphics_feed */
                if (p->b) {
                    if (p->a == 2) graphics_gif_feed(3, words, p->b);
                    else vif_feed((int)p->a, words, p->b);
                }
                break;
            case WQ_FRAME:
                web_frame(args[0]);
                break;
            case WQ_VSYNC:
                web_gs_mmio_snapshot = args;
                q->display = web_display();
                web_gs_mmio_snapshot = 0;
                answer = WQ_RUN_PRESENT;
                break;
            case WQ_ENV:
                if (p->a >= sizeof env_copy) __builtin_trap();
                __builtin_memcpy(env_copy, p + 1, p->a);
                env_copy[p->a] = 0;
                q->env_text = (uint32_t)(uintptr_t)env_copy;
                q->env_bytes = p->a;
                answer = WQ_RUN_ENV;
                break;
            default:
                __builtin_trap();
            }
        }
        at += bytes;
        store(&q->consumed, at);
        if (load(&q->waiting)) __builtin_wasm_memory_atomic_notify((int *)&q->consumed, 0xffffffffu);
        if (answer >= 0) return answer;
    }
}

/* The vsync's picture onto the canvas (worker.js does this in the one-worker build, in IEEE mode,
   which the reactor's word is in at rest). */
__attribute__((export_name("web_q_present"))) int web_q_present(uint32_t canvas_w, uint32_t canvas_h)
{
    return web_present(canvas_w, canvas_h);
}
#endif
