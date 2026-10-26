/* The platform under the native renderer's shared half (native_core.inc): its locks, condition
   variable, render thread, atomic stores, waitable timer, bit scan, process exit and the window
   handles it carries for the backend. native_core.inc calls only these, so it compiles wherever
   this header has an implementation.

     Windows        CRITICAL_SECTION, CONDITION_VARIABLE, _beginthreadex, InterlockedExchange,
                    a high-resolution waitable timer, _BitScanForward, ExitProcess: exactly what
                    native_core.inc called directly before.
     NATIVE_INLINE  one thread, nothing to lock (a later browser native mode, finding 085): the
                    locks are no-ops, there is no render thread (native_core.inc draws each field
                    at its own vsync), and a wait that would need another thread to end it stops
                    the process instead of hanging.

   Linux, Android and iOS get a pthread implementation here later (PLAN.md §1.4); nothing in
   native_core.inc changes for them. NativeWindow is present.c's window: HWND on Windows,
   elsewhere opaque. */
#ifndef PORT_GS_NATIVE_THREAD_H
#define PORT_GS_NATIVE_THREAD_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(NATIVE_INLINE)

#include <strings.h>

typedef int NativeMutex;
typedef int NativeCond;
typedef int NativeThread;
typedef int NativeTimer;
typedef void *NativeWindow;
#define NATIVE_PATH_MAX 4096

static inline void native_mutex_init(NativeMutex *m) { (void)m; }
static inline void native_mutex_lock(NativeMutex *m) { (void)m; }
static inline void native_mutex_unlock(NativeMutex *m) { (void)m; }
static inline void native_cond_init(NativeCond *c) { (void)c; }
static inline void native_cond_broadcast(NativeCond *c) { (void)c; }
/* Only another thread could end this wait, and there is none. */
static inline void native_cond_wait(NativeCond *c, NativeMutex *m)
{
    (void)c; (void)m;
    fprintf(stderr, "gs native: a wait with no render thread (NATIVE_INLINE)\n");
    fflush(stderr);
    abort();
}
static inline void native_atomic_store(volatile long *target, long value) { __atomic_store_n(target, value, __ATOMIC_SEQ_CST); }
static inline NativeTimer native_timer_create(void) { return 0; }
static inline int native_timer_wait(NativeTimer timer, double seconds) { (void)timer; (void)seconds; return 0; }
static inline void native_sleep_ms(unsigned ms) { (void)ms; }
static inline void native_cpu_relax(void) {}
static inline unsigned native_bit_first(uint32_t bits) { return (unsigned)__builtin_ctz(bits); }
static inline void native_exit_now(int code) { _Exit(code); }
static inline int native_stricmp(const char *a, const char *b) { return strcasecmp(a, b); }

#elif defined(_WIN32)

#include <windows.h>
#include <process.h>
#include <intrin.h>

typedef CRITICAL_SECTION NativeMutex;
typedef CONDITION_VARIABLE NativeCond;
typedef HANDLE NativeThread;
typedef HANDLE NativeTimer;
typedef HWND NativeWindow;
#define NATIVE_PATH_MAX MAX_PATH
#define NATIVE_THREAD_RESULT unsigned __stdcall
typedef unsigned (__stdcall *NativeThreadProc)(void *argument);

static inline void native_mutex_init(NativeMutex *m) { InitializeCriticalSection(m); }
static inline void native_mutex_lock(NativeMutex *m) { EnterCriticalSection(m); }
static inline void native_mutex_unlock(NativeMutex *m) { LeaveCriticalSection(m); }
static inline void native_cond_init(NativeCond *c) { InitializeConditionVariable(c); }
static inline void native_cond_broadcast(NativeCond *c) { WakeAllConditionVariable(c); }
static inline void native_cond_wait(NativeCond *c, NativeMutex *m) { SleepConditionVariableCS(c, m, INFINITE); }

/* The render thread, above normal priority so a picture is not held up by the EE's threads. */
static inline NativeThread native_thread_start(NativeThreadProc proc, void *argument)
{
    HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, proc, argument, 0, NULL);
    SetThreadPriority(thread, THREAD_PRIORITY_ABOVE_NORMAL);
    return thread;
}

/* A store the other threads see whole (native surface counts and extents only grow). */
static inline void native_atomic_store(volatile long *target, long value) { InterlockedExchange(target, value); }

/* High resolution where Windows has it (10 1803 and later); 0 (sleep instead) where it fails. */
static inline NativeTimer native_timer_create(void)
{
    return CreateWaitableTimerExW(NULL, NULL, 0x00000002 /* CREATE_WAITABLE_TIMER_HIGH_RESOLUTION */,
                                  TIMER_ALL_ACCESS);
}

/* Waits about this many seconds on the timer; 0 when it could not be set (nothing waited). */
static inline int native_timer_wait(NativeTimer timer, double seconds)
{
    LARGE_INTEGER due;
    due.QuadPart = -(LONGLONG)(seconds * 1e7);
    if (!SetWaitableTimer(timer, &due, 0, NULL, NULL, FALSE)) return 0;
    WaitForSingleObject(timer, INFINITE);
    return 1;
}

static inline void native_sleep_ms(unsigned ms) { Sleep((DWORD)ms); }
static inline void native_cpu_relax(void) { YieldProcessor(); }

/* The lowest set bit's index; bits is never 0. */
static inline unsigned native_bit_first(uint32_t bits)
{
    unsigned long bit;
    _BitScanForward(&bit, bits);
    return (unsigned)bit;
}

/* Leaves at once, without atexit handlers (the exit report would wait on the render thread). */
static inline void native_exit_now(int code) { ExitProcess((UINT)code); }
static inline int native_stricmp(const char *a, const char *b) { return _stricmp(a, b); }

#else
#error "native_thread.h: no implementation for this platform yet (define NATIVE_INLINE for one thread)"
#endif

#endif
