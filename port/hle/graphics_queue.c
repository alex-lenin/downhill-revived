/* Copied VIF1/GIF packets run in FIFO order on one optional graphics worker.
   Interlocked publication owns the ring bytes until the consumer retires them.
   Design reference: PCSX2 MTVU's copied command ring; no source copied. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <xmmintrin.h>
#include "hle.h"
#include "../gs/gs.h"
#include "graphics_profile.h"

#ifdef GRAPHICS_QUEUE_TEST
enum { RING_BYTES = 4096 };
#else
enum { RING_BYTES = 8 * 1024 * 1024 };
#endif

typedef struct { uint32_t bytes, unit, words, padding; } Packet;
static uint8_t *ring;
static HANDLE worker, available, retired;
static volatile LONG written, consumed, stopping;
/* Each side signals only when the other sleeps: an event per packet (thousands a
   frame) cost a kernel call and a thread wake each. */
static volatile LONG sleeping, waiting;
static int initialized;
static int adaptive_waits;

/* Reads only acquire ownership published by the other side. A locked RMW here
   unnecessarily takes exclusive ownership of its cache line on every poll.
   Keep the interlocked publications and sleep announcements below: their full
   barriers also order each publication before the subsequent waiter check. */
static uint32_t position(volatile LONG *p) { return (uint32_t)__atomic_load_n(p, __ATOMIC_ACQUIRE); }

/* Spin briefly before sleeping until *p moves away from at. *flag announces the sleep;
   the interlocked store and reread pair with the other side's publish-then-read. */
static void idle_wait(volatile LONG *p, uint32_t at, volatile LONG *flag, HANDLE event, DWORD timeout, unsigned *budget)
{
    unsigned limit = budget && adaptive_waits ? *budget : 4096;
    for (unsigned spin = 0; spin < limit; spin++) {
        if (position(p) != at) {
            if (budget && adaptive_waits) {
                unsigned next = spin * 2 + 128;
                *budget = next < 4096 ? next : 4096;
            }
            return;
        }
        _mm_pause();
    }
    /* Only empty consumer waits adapt. Retired/full waits keep their original
       budget and timeout. Publication and lost-wakeup handshake are unchanged. */
    if (budget && adaptive_waits) *budget = limit > 256 ? limit / 2 : 128;
    InterlockedIncrement(flag);
    if (position(p) == at) WaitForSingleObject(event, timeout);
    InterlockedDecrement(flag);
}

/* Retired-side waits share one auto-reset event; the short timeout covers two waiters. */
static void wait_retired(void) { idle_wait(&consumed, position(&consumed), &waiting, retired, 1, NULL); }

#include "graphics_timeline.h"

#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
#include "gs_queue.inc"
#endif

static void execute(unsigned unit, const uint32_t *words, unsigned count)
{
    /* VIF1/GIF execute VU1 and host rendering, not EE game code. Host IEEE
       exceptional values must not inherit the game's diagnostic SSE mask. */
    unsigned control = _mm_getcsr();
    _mm_setcsr((control | 0x1F80u) & ~(63u | 0xE040u));  /* IEEE: nearest, no FTZ/DAZ */
    if (unit == 2) {
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
        double start = worker_profile_start && worker_profile ? host_wall_seconds() : 0;
        graphics_gif_feed(3, words, count);
        if (start) vif_gs_seconds += host_wall_seconds()-start;
#else
        gs_feed(&gs, 3, words, count);
#endif
    }
    else vif_feed((int)unit, words, count);
    _mm_setcsr(control);
}

static DWORD WINAPI consume(void *unused)
{
    (void)unused;
    host_thread_name("vu");
    unsigned idle_budget = 1024;
    for (;;) {
        uint32_t at = position(&consumed);
        if (at == position(&written)) {
            if (position(&stopping)) return 0;
            LONG64 idle = tl_begin();
            idle_wait(&written, at, &sleeping, available, INFINITE, &idle_budget);
            tl_add(TL_VU_IDLE, idle);
            continue;
        }
        const Packet *packet = (const Packet *)(ring + (at & (RING_BYTES - 1)));
        if (packet->unit != UINT32_MAX) {
            double start = worker_begin();
            execute(packet->unit, (const uint32_t *)(packet + 1), packet->words);
            worker_end(start);
        }
        InterlockedExchange(&consumed, (LONG)(at + packet->bytes));
        if (position(&waiting)) SetEvent(retired);
    }
}

void graphics_wait(void)
{
    if (!worker) return;
    uint32_t target = position(&written);
    LONG64 start = tl_begin();
    int pending = (int32_t)(position(&consumed) - target) < 0;
    while ((int32_t)(position(&consumed) - target) < 0)
        wait_retired();
    tl_add(TL_EE_VU, start);
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
    LONG64 gs_start = tl_begin();
    pending |= gs_position(&gs_consumed) != gs_position(&gs_written);
    graphics_gs_wait();
    tl_add(TL_EE_GS, gs_start);
#endif
    if (start) tl_site(__builtin_return_address(0), pending, tl_now() - start);
}

void graphics_shutdown(void)
{
    if (!worker) return;
    graphics_wait();
    InterlockedExchange(&stopping, 1);
    SetEvent(available);
    WaitForSingleObject(worker, INFINITE);
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
    gs_queue_shutdown();
#endif
    CloseHandle(worker); CloseHandle(available); CloseHandle(retired);
    free(ring);
    worker = available = retired = NULL; ring = NULL;
}

/* The default since round 22: race frames and dumps are identical to inline execution
   (finding 030). DHD_MTVU=0 keeps everything on the EE thread. */
int graphics_threaded(void)
{
    const char *option = getenv("DHD_MTVU");
    return !option || strcmp(option, "0");
}

static void initialize(void)
{
    initialized = 1;
    /* DHD_FIFO_ADAPTIVE=0 keeps the fixed 4096-spin budget for the consumers' empty waits (finding 069). */
    const char *adaptive = getenv("DHD_FIFO_ADAPTIVE");
    adaptive_waits = !adaptive || strcmp(adaptive, "0");
    if (!graphics_threaded()) return;
#ifdef GRAPHICS_QUEUE_TEST
    /* Cross the 32-bit publication counter rollover during the stress test. */
    written = consumed = (LONG)0xFFFFF800u;
#endif
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
    gs_queue_initialize();
#endif
    ring = malloc(RING_BYTES);
    available = CreateEventA(NULL, FALSE, FALSE, NULL);
    retired = CreateEventA(NULL, FALSE, FALSE, NULL);
    if (ring && available && retired) worker = CreateThread(NULL, 0, consume, NULL, 0, NULL);
    if (!worker) {
        if (available) CloseHandle(available);
        if (retired) CloseHandle(retired);
        free(ring); ring = NULL; available = retired = NULL;
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
        gs_queue_shutdown();
#endif
        hle_log("graphics: worker unavailable, using inline execution\n");
    } else {
        atexit(graphics_shutdown);
        hle_log("graphics: VIF1/GIF worker enabled\n");
    }
}

void graphics_feed(int unit, const uint32_t *words, uint32_t count)
{
    if (unit == 0) { vif_feed(unit, words, count); return; }
    if (!initialized) initialize();
    if (!worker) { execute((unsigned)unit, words, count); return; }
    while (count) {
        unsigned take = count > RING_BYTES / 16 ? RING_BYTES / 16 : count;
        unsigned bytes = (sizeof(Packet) + take * 4 + 15) & ~15u;
        uint32_t at = position(&written);
        unsigned tail = RING_BYTES - (at & (RING_BYTES - 1));
        unsigned wrap = bytes > tail ? tail : 0;
        if (RING_BYTES - (at - position(&consumed)) < bytes + wrap) {
            LONG64 full = tl_begin();
            while (RING_BYTES - (at - position(&consumed)) < bytes + wrap)
                wait_retired();
            tl_add(TL_EE_FULL, full);
        }
        if (wrap) {
            Packet *padding = (Packet *)(ring + (at & (RING_BYTES - 1)));
            *padding = (Packet){wrap, UINT32_MAX, 0, 0};
            at += wrap;
        }
        Packet *packet = (Packet *)(ring + (at & (RING_BYTES - 1)));
        *packet = (Packet){bytes, (unsigned)unit, take, 0};
        memcpy(packet + 1, words, take * 4);
        InterlockedExchange(&written, (LONG)(at + bytes));
        if (position(&sleeping)) SetEvent(available);
        words += take; count -= take;
    }
}
