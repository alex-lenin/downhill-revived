/* CPU clocks and placement for the game's threads on Android (finding 091, AND2). The game's work
   per field is split over a pipeline of threads (game -> vu -> gs -> native) that wait on each
   other, so none keeps a core busy; the scheduler moves them between clusters and the governor
   clocks each cluster by its load, so a thread can land on a core running at a fraction of its
   speed (the Mi 11i's X1 sat at 0.84 GHz with the game thread on it).

   - Performance hints (ADPF, APerformanceHint, API 33): one hint session holds the pipeline's
     threads; each field the game thread reports the longest CPU time any of them spent on the last
     field against a target, and a system with hint support raises their clocks until it fits. The
     calls come from libandroid.so through dlsym (the APK's minimum is API 30); without them, or when
     the system has no hint support (createSession answers NULL, as on the Mi 11i's MIUI 14), nothing
     happens. DHD_PERF_HINT = 0 turns it off; DHD_PERF_TARGET_MS sets the target (default 13: under
     one 60 Hz field, with room for the hand-offs).
   - Placement: DHD_AFFINITY = name:cpus,... (cpus: 7, or 4-6, or 4+7) keeps a named thread on those
     cores. Applied from the game thread by thread ID and again every 10 fields, because the system
     moves threads off their cores (cpuset moves; on the Mi 11i something resets it every few seconds).

   Only clocks and placement change: what the threads compute is the same. */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct APerformanceHintManager APerformanceHintManager;
typedef struct APerformanceHintSession APerformanceHintSession;

static APerformanceHintManager *(*get_manager)(void);
static APerformanceHintSession *(*create_session)(APerformanceHintManager *, const int32_t *, size_t, int64_t);
static int (*report_actual)(APerformanceHintSession *, int64_t);
static void (*close_session)(APerformanceHintSession *);

/* The pipeline's threads, as they name themselves (host_thread_name). */
static const char *const hot[] = {"game", "vu", "gs", "native"};
enum { HOT = sizeof hot / sizeof hot[0] };
static struct { int32_t tid; clockid_t clock; volatile int set; } threads[HOT];

/* host_thread_name (port/hle/host.c), on the thread itself. */
void android_perf_thread(const char *name)
{
    for (int i = 0; i < HOT; i++) {
        if (strcmp(name, hot[i]) || threads[i].set) continue;
        threads[i].tid = gettid();
        if (pthread_getcpuclockid(pthread_self(), &threads[i].clock) == 0)
            __atomic_store_n(&threads[i].set, 1, __ATOMIC_RELEASE);
        return;
    }
}

static int64_t cpu_ns(clockid_t clock)
{
    struct timespec t;
    return clock_gettime(clock, &t) ? 0 : (int64_t)t.tv_sec * 1000000000 + t.tv_nsec;
}

/* DHD_AFFINITY's cores for thread i: "7", "4-6" or "4+7" after "name:". 0 when not named. */
static int pin_set(const char *spec, int i, cpu_set_t *set)
{
    size_t len = strlen(hot[i]);
    CPU_ZERO(set);
    for (const char *at = spec; at && *at; at = strchr(at, ',') ? strchr(at, ',') + 1 : NULL) {
        if (strncmp(at, hot[i], len) || at[len] != ':') continue;
        const char *p = at + len + 1;
        int any = 0;
        while (*p >= '0' && *p <= '9') {
            char *end;
            long lo = strtol(p, &end, 10), hi = lo;
            if (*end == '-') hi = strtol(end + 1, &end, 10);
            for (long c = lo; c <= hi && c < CPU_SETSIZE; c++) { CPU_SET((int)c, set); any = 1; }
            p = *end == '+' ? end + 1 : end;
        }
        return any;
    }
    return 0;
}

static void place(unsigned field)
{
    static const char *spec;
    static int read, reported, failures;
    if (!read) { spec = getenv("DHD_AFFINITY"); read = 1; }
    if (!spec || field % 10) return;
    for (int i = 0; i < HOT; i++) {
        cpu_set_t set, now;
        if (!__atomic_load_n(&threads[i].set, __ATOMIC_ACQUIRE) || !pin_set(spec, i, &set)) continue;
        int moved = sched_getaffinity(threads[i].tid, sizeof now, &now) == 0 && !CPU_EQUAL(&now, &set);
        int failed = sched_setaffinity(threads[i].tid, sizeof set, &set) ? errno : 0;
        if ((!(reported & (1 << i)) && field >= 600) || ((moved || failed) && field >= 600 && failures < 8)) {
            fprintf(stderr, "android: field %u: %s thread (tid %d) %s%s%s\n", field, hot[i], threads[i].tid,
                    moved ? "had been moved off its cores; " : "", failed ? "could not be placed: " : "placed",
                    failed ? strerror(failed) : "");
            reported |= 1 << i;
            failures += moved || failed;
        }
    }
}

static void hint(void)
{
    static int state;  /* 0 unread, 1 waiting for the threads, 2 reporting, 3 off */
    static APerformanceHintSession *session;
    static int64_t target, before[HOT];
    static int members;
    if (state == 3) return;
    if (!state) {
        const char *on = getenv("DHD_PERF_HINT"), *ms = getenv("DHD_PERF_TARGET_MS");
        void *lib = (on && !strcmp(on, "0")) ? NULL : dlopen("libandroid.so", RTLD_NOW);
        if (lib) {
            get_manager = (APerformanceHintManager * (*)(void)) dlsym(lib, "APerformanceHint_getManager");
            create_session = (APerformanceHintSession * (*)(APerformanceHintManager *, const int32_t *, size_t,
                                                            int64_t)) dlsym(lib, "APerformanceHint_createSession");
            report_actual = (int (*)(APerformanceHintSession *, int64_t))
                dlsym(lib, "APerformanceHint_reportActualWorkDuration");
            close_session = (void (*)(APerformanceHintSession *)) dlsym(lib, "APerformanceHint_closeSession");
        }
        if (!get_manager || !create_session || !report_actual || !close_session) {
            fprintf(stderr, "android: performance hints %s\n", lib ? "not available (API < 33)" : "off");
            state = 3;
            return;
        }
        double t = ms ? atof(ms) : 13.0;
        target = (int64_t)((t > 1 && t < 100 ? t : 13.0) * 1e6);
        state = 1;
    }
    int n = 0;
    int32_t tids[HOT];
    for (int i = 0; i < HOT; i++)
        if (__atomic_load_n(&threads[i].set, __ATOMIC_ACQUIRE)) tids[n++] = threads[i].tid;
    if (n != members) {  /* a pipeline thread started: a new session (API 33 can't add threads to one) */
        if (session) close_session(session);
        APerformanceHintManager *manager = get_manager();
        session = manager ? create_session(manager, tids, (size_t)n, target) : NULL;
        if (!session) {
            fprintf(stderr, "android: no performance hint session (the system has no hint support)\n");
            state = 3;
            return;
        }
        fprintf(stderr, "android: performance hint session: %d threads, target %.1f ms\n", n, target / 1e6);
        members = n;
        for (int i = 0; i < HOT; i++)
            if (threads[i].set) before[i] = cpu_ns(threads[i].clock);
        state = 2;
        return;
    }
    if (state != 2) return;
    int64_t longest = 0;
    for (int i = 0; i < HOT; i++) {
        if (!threads[i].set) continue;
        int64_t now = cpu_ns(threads[i].clock), spent = now - before[i];
        before[i] = now;
        if (spent > longest) longest = spent;
    }
    if (longest > 0) report_actual(session, longest);
}

/* gs_vsync (port/gs/present.c), on the game thread, once a field. */
void android_perf_field(void)
{
    static unsigned field;
    place(field++);
    hint();
}
