/* The part of the 989snd host output that the device backends share: sound_out_win32.c (WASAPI,
   the Windows build) and sound_out_sdl.c (SDL3 audio, Linux with DHD_SOUND=sdl) each include it
   once; a build compiles exactly one backend (host64_census.HOST_UNITS, build_host64_linux.py).
   It holds the ring the game thread fills (host_sound_write) and the backend's thread drains,
   the output counters and histograms, the DHD_BENCH "bench snd:" line and the parts of the
   interface (host_api.h, "sound output") that don't touch the device. The backend defines
   host_start (start its output thread) and host_bench_tag (the end of the bench line). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "host_api.h"
#include "../hle/hle.h"

#define SND_RATE HOST_SOUND_RATE
#define RING_SAMPLES (SND_RATE / 2)
#define DEVICE_MS 30
#define SKIP_MS 120 /* queued beyond the cushion: skip ahead */
#define RATE_RANGE 0.005
static int16_t ring[RING_SAMPLES * 2];
static volatile long ring_read, ring_write;
static int host_started;
static volatile long host_underruns, host_silence, host_dropped, host_skipped, host_frames;
static volatile long host_queue_hist[512]; /* queued ms at each device top-up */
static volatile long host_tick_hist[128];  /* game tick interval, 0.5-ms bins */
static volatile int host_hist_reset;
static volatile double host_ratio = 1.0;
static volatile long host_stretched;       /* output frames made by a time stretcher (WASAPI only) */

static void host_start(void);
static const char *host_bench_tag(void);

static int trace_on(void)
{
    static int on = -1;
    if (on < 0) on = getenv("DHD_SND_TRACE") != NULL;
    return on;
}

static void ring_push(const int16_t *stereo, int samples)
{
    int i;
    for (i = 0; i < samples; i++) {
        long w = ring_write;
        if (w - ring_read >= RING_SAMPLES) {
            /* host is behind: drop; game state already advanced */
            host_dropped += samples - i;
            return;
        }
        ring[(w % RING_SAMPLES) * 2] = stereo[i * 2];
        ring[(w % RING_SAMPLES) * 2 + 1] = stereo[i * 2 + 1];
        ring_write = w + 1;
    }
}

static int host_env_ms(const char *name, int fallback, int low, int high)
{
    const char *v = getenv(name);
    int ms = v && *v ? atoi(v) : fallback;
    return ms < low ? low : ms > high ? high : ms;
}

static void host_histograms(unsigned queued)
{
    unsigned ms = queued / (SND_RATE / 1000);
    if (host_hist_reset) {
        memset((void *)host_queue_hist, 0, sizeof host_queue_hist);
        host_hist_reset = 0;
    }
    host_queue_hist[ms < 511 ? ms : 511]++;
}

static unsigned host_percentile(volatile long *hist, unsigned bins, double fraction)
{
    unsigned i;
    long total = 0, seen = 0;
    for (i = 0; i < bins; i++) total += hist[i];
    for (i = 0; i < bins; i++) {
        seen += hist[i];
        if (total && seen >= total * fraction) return i;
    }
    return 0;
}

/* DHD_BENCH window start (0) and end (1), called by present.c. */
void snd_host_bench(int end)
{
    static long underruns, silence, dropped, skipped, frames, stretched;
    const double per_ms = SND_RATE / 1000.0;
    if (!end) {
        underruns = host_underruns; silence = host_silence; dropped = host_dropped;
        skipped = host_skipped; frames = host_frames; stretched = host_stretched;
        host_hist_reset = 1;
        memset((void *)host_tick_hist, 0, sizeof host_tick_hist);
        return;
    }
    fprintf(stderr, "bench snd: underruns=%ld silence_ms=%.1f dropped_ms=%.1f skipped_ms=%.1f played_s=%.2f "
            "queue_ms p1=%u p50=%u p99=%u rate_ppm=%+.0f tick_ms p50=%.1f p99=%.1f max=%.1f stretched_s=%.2f%s\n",
            host_underruns - underruns, (host_silence - silence) / per_ms, (host_dropped - dropped) / per_ms,
            (host_skipped - skipped) / per_ms, (host_frames - frames) / (double)SND_RATE,
            host_percentile(host_queue_hist, 512, 0.01), host_percentile(host_queue_hist, 512, 0.5),
            host_percentile(host_queue_hist, 512, 0.99), (host_ratio - 1) * 1e6,
            host_percentile(host_tick_hist, 128, 0.5) * 0.5, host_percentile(host_tick_hist, 128, 0.99) * 0.5,
            host_percentile(host_tick_hist, 128, 1.0) * 0.5, (host_stretched - stretched) / (double)SND_RATE,
            host_bench_tag());
}

/* ---- the interface (host_api.h, "sound output") ---- */

void host_sound_open(void)
{
    if (host_started)
        return;
    host_started = 1;
    if (!getenv("DHD_SND_OFF"))
        host_start();
}

void host_sound_write(const int16_t *stereo, int frames)
{
    ring_push(stereo, frames);
}

void host_sound_reset(void)
{
    ring_read = ring_write = 0;
}

/* Wall-clock diagnostics: how evenly the game thread renders (DHD_BENCH only). */
void host_sound_tick(void)
{
    static double previous;
    double now = host_wall_seconds();
    if (previous > 0) {
        unsigned bin = (unsigned)((now - previous) * 2000);
        host_tick_hist[bin < 127 ? bin : 127]++;
    }
    previous = now;
}
