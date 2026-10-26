/* Host side of the 989snd output where there is no audio device (Linux
   headless): the mixer's PCM is counted and dropped. Interface: host_api.h,
   "sound output". With DHD_SND_OFF the mixer still runs and hands its frames
   over; without a device nothing plays them. The DHD_BENCH counters keep
   their format so tooling parses the same line. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "host_api.h"
#include "../hle/hle.h"

static int host_started;
static volatile long host_dropped, host_frames;
static volatile double host_ratio = 1.0;
static volatile long host_stretched;

/* DHD_BENCH window start (0) and end (1), called by present.c. */
void snd_host_bench(int end)
{
    static long dropped, frames, stretched;
    if (!end) {
        dropped = host_dropped;
        frames = host_frames;
        stretched = host_stretched;
        return;
    }
    fprintf(stderr, "bench snd: underruns=0 silence_ms=0.0 dropped_ms=%.1f skipped_ms=0.0 played_s=%.2f "
            "queue_ms p1=0 p50=0 p99=0 rate_ppm=%+.0f tick_ms p50=0.0 p99=0.0 max=0.0 stretched_s=%.2f\n",
            (host_dropped - dropped) / (HOST_SOUND_RATE / 1000.0),
            (host_frames - frames) / (double)HOST_SOUND_RATE,
            (host_ratio - 1) * 1e6, (host_stretched - stretched) / (double)HOST_SOUND_RATE);
}

/* ---- the interface (host_api.h, "sound output") ---- */

void host_sound_open(void)
{
    /* No device to open; the mixer runs either way. */
    host_started = 1;
}

void host_sound_write(const int16_t *stereo, int frames)
{
    (void)stereo;
    if (host_started) host_frames += frames;
}

void host_sound_reset(void) {}

/* Wall-clock diagnostics: how evenly the game thread renders (DHD_BENCH only). */
void host_sound_tick(void) {}

void host_sound_log_stack(void)
{
    hle_log("989snd: unresolved-play stack: none (no audio device)\n");
}
