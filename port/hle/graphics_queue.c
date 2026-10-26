/* Copied VIF1/GIF packets run in FIFO order on one optional graphics worker.
   Interlocked publication owns the ring bytes until the consumer retires them.
   Design reference: PCSX2 MTVU's copied command ring; no source copied. */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include "../host/win32_compat.h"
#endif
#include <stdlib.h>
#include <string.h>
#if defined(__aarch64__)
/* The Android build (finding 091): FPCR for MXCSR; isb for pause: yield is a no-op on the Cortex
   cores, so the 4096-step spins before a sleep lasted ~2 us instead of x86's ~100 us and the
   workers slept and were woken (a futex call each side) far more often (AND2). */
static inline unsigned fp_control(void) { unsigned long v; __asm__ volatile("mrs %0, fpcr" : "=r"(v)); return (unsigned)v; }
/* skipped when unchanged (the usual case on the workers): the write and its isb serialize the core */
static inline void fp_set_control(unsigned v) { if (v != fp_control()) __asm__ volatile("msr fpcr, %0\n\tisb" :: "r"((unsigned long)v) : "memory"); }
/* IEEE: nearest (RMode 0), no flush to zero (FZ, FZ16), no default NaN, no AH/FIZ/NEP, no traps */
#define FP_IEEE(c) ((c) & ~((3u << 22) | (1u << 24) | (1u << 25) | (1u << 19) | 0x9F07u))
#define cpu_pause() __asm__ volatile("isb" ::: "memory")
#else
#include <xmmintrin.h>
#define fp_control() _mm_getcsr()
#define fp_set_control(v) _mm_setcsr(v)
#define FP_IEEE(c) (((c) | 0x1F80u) & ~(63u | 0xE040u))  /* IEEE: nearest, no FTZ/DAZ */
#define cpu_pause() _mm_pause()
#endif
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
/* Producer/consumer writes must not invalidate each other's publication line.
   Keep announcements separate too: both threads poll them for every packet. */
#define QUEUE_COUNTER(name) static volatile LONG name __attribute__((aligned(64)))
QUEUE_COUNTER(written);
QUEUE_COUNTER(consumed);
QUEUE_COUNTER(stopping);
/* Each side signals only when the other sleeps: an event per packet (thousands a
   frame) cost a kernel call and a thread wake each. */
QUEUE_COUNTER(sleeping);
QUEUE_COUNTER(waiting);
static int initialized;
static int adaptive_waits;
static int spike_waits;
static double spin_seconds = -1;  /* DHD_SPIN_US, read at the first long wait (idle_wait) */

/* Reads only acquire ownership published by the other side. A locked RMW here
   unnecessarily takes exclusive ownership of its cache line on every poll.
   Keep the interlocked publications and sleep announcements below: their full
   barriers also order each publication before the subsequent waiter check. */
static uint32_t position(volatile LONG *p) { return (uint32_t)__atomic_load_n(p, __ATOMIC_ACQUIRE); }

#include "graphics_timeline.h"

/* Spin briefly before sleeping until *p moves away from at. *flag announces the sleep;
   the interlocked store and reread pair with the other side's publish-then-read. */
static void idle_wait(volatile LONG *p, uint32_t at, volatile LONG *flag, HANDLE event, DWORD timeout, unsigned *budget)
{
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
    LONG64 spin_start = tl_begin();
#endif
    unsigned limit = budget ? (adaptive_waits ? *budget : 4096) : 512;
    for (unsigned spin = 0; spin < limit; spin++) {
        if (position(p) != at) {
            if (budget && adaptive_waits) {
                unsigned next = spin * 2 + 128;
                *budget = next < 4096 ? next : 4096;
            }
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
            tl_add(TL_EE_SPIN + 2 * tl_role, spin_start);
#endif
            return;
        }
        cpu_pause();
    }
    /* DHD_SPIN_US (finding 091 AND2): the producer's waits (retired/full, the game thread's) spin
       that long before sleeping, so a phone's governor sees a busy core and keeps its clock up.
       Android's default 3000 (+4% race fields/s on the Mi 11i, p99 down); elsewhere 0. */
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
    if (!budget && spin_seconds) {
        if (spin_seconds < 0) {
            const char *us = getenv("DHD_SPIN_US");
#ifdef __ANDROID__
            spin_seconds = (us ? atof(us) : 3000) * 1e-6;
#else
            spin_seconds = us ? atof(us) * 1e-6 : 0;
#endif
        }
        for (double end = spin_seconds > 0 ? host_wall_seconds() + spin_seconds : 0; end;) {
            for (unsigned spin = 0; spin < 256; spin++) {
                if (position(p) != at) {
                    tl_add(TL_EE_SPIN + 2 * tl_role, spin_start);
                    return;
                }
                cpu_pause();
            }
            if (host_wall_seconds() >= end) break;
        }
    }
#endif
    /* Only empty consumer waits adapt. Retired/full waits spin at most 512 pauses
       before parking. Their timeout and lost-wakeup handshake are unchanged. */
    if (budget && adaptive_waits) *budget = limit > 256 ? limit / 2 : 128;
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
    tl_add(TL_EE_SPIN + 2 * tl_role, spin_start);
#endif
    InterlockedIncrement(flag);
    if (position(p) == at) {
#ifndef GRAPHICS_QUEUE_TEST
        double start = spike_waits ? host_wall_seconds() : 0;
#endif
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
        LONG64 park_start = tl_begin();
#endif
        DWORD result = WaitForSingleObject(event, timeout);
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
        tl_add(TL_EE_PARK + 2 * tl_role, park_start);
#endif
#ifndef GRAPHICS_QUEUE_TEST
        if (spike_waits && result == WAIT_TIMEOUT)
            fprintf(stderr, "spike_queue: start=%.9f ms=%.6f moved=%u\n", start,
                    (host_wall_seconds()-start)*1e3, position(p) != at);
#endif
    }
    InterlockedDecrement(flag);
}

/* Retired-side waits share one auto-reset event; the short timeout covers two waiters. */
/* Wait on the same observation that failed the caller's predicate. A fresh load here
   can already equal its target: no packet follows it until the EE starts the next field. */
static void wait_retired(uint32_t at) { idle_wait(&consumed, at, &waiting, retired, 1, NULL); }

#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
#include "gs_queue.inc"
#endif

static void execute(unsigned unit, const uint32_t *words, unsigned count)
{
    /* VIF1/GIF execute VU1 and host rendering, not EE game code. Host IEEE
       exceptional values must not inherit the game's diagnostic SSE mask. */
    unsigned control = fp_control();
    fp_set_control(FP_IEEE(control));  /* IEEE: nearest, no FTZ/DAZ */
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
    fp_set_control(control);
}

static DWORD WINAPI consume(void *unused)
{
    (void)unused;
    host_thread_name("vu");
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
    tl_role = 1;
#endif
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
    uint32_t observed;
    while ((int32_t)((observed = position(&consumed)) - target) < 0)
        wait_retired(observed);
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
    spike_waits = getenv("DHD_SPIKES") != NULL;
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

void graphics_feed(int unit, uint32_t guest_words, uint32_t count)
{
    const uint32_t *words = GUEST(guest_words);
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
            uint32_t observed;
            while (RING_BYTES - (at - (observed = position(&consumed))) < bytes + wrap)
                wait_retired(observed);
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

/* Guest-address-only module boundary; native device code stays above. */
#include "../host/host_api.h"
#include "../include/guest.h"
void host_service_graphics_feed(int a0, uint32_t a1, uint32_t a2) { graphics_feed(a0, a1, a2); }
void host_service_graphics_wait(void) { graphics_wait(); }
int host_service_graphics_threaded(void) { return graphics_threaded(); }
void host_service_graphics_shutdown(void) { graphics_shutdown(); }
