/* Host side of the 989snd output through SDL3's audio (W11): an SDL audio device stream in place of
   WASAPI (sound_out_win32.c). The same job, the same format (48000 Hz, stereo, 16-bit PCM), the
   same targets: the game (sound.c) renders into a ring, and a host thread keeps a cushion of
   DHD_SND_LATENCY_MS (default 70) queued, fills the device only to DEVICE_MS (30), skips ahead
   beyond the cushion by SKIP_MS (120), and follows the clock drift by reading the ring up to
   0.5% faster or slower. Built instead of sound_out_null.c (Linux) or sound_out_win32.c
   (Windows) with DHD_SOUND=sdl at build time.

   Differences (open in docs/audit w11-sdl3.md): no WSOLA time-stretch (a game running slow plays
   gaps where WASAPI stretches); the queue measured is the stream's, so SDL's own device buffer
   adds to the latency and is not counted in the cushion. No device is opened in headless runs
   (DHD_HEADLESS) or with DHD_SND_OFF. The ring, the counters and the bench line are
   sound_out_common.inc's, shared with sound_out_win32.c. Interface: port/host/host_api.h,
   "sound output". */
#include <SDL3/SDL.h>

#include "sound_out_common.inc"

#define CHUNK_FRAMES (DEVICE_MS * (SND_RATE / 1000))

/* The ring read at `ratio` input frames per output frame, linearly interpolated (the copy path of
   sound_out_win32.c's st_fill + host_resample). Returns the frames it made. */
static unsigned host_resample(int16_t *out, unsigned count, double ratio, double *phase)
{
    unsigned i;
    for (i = 0; i < count; i++) {
        long r = ring_read;
        double f = *phase;
        if (ring_write - r < 2) break;
        const int16_t *a = &ring[(r % RING_SAMPLES) * 2], *b = &ring[((r + 1) % RING_SAMPLES) * 2];
        out[i * 2] = (int16_t)(a[0] + (b[0] - a[0]) * f);
        out[i * 2 + 1] = (int16_t)(a[1] + (b[1] - a[1]) * f);
        f += ratio;
        while (f >= 1.0) { f -= 1.0; r++; }
        *phase = f;
        ring_read = r;
    }
    return i;
}

static void host_loop(SDL_AudioStream *stream)
{
    const unsigned per_ms = SND_RATE / 1000;
    unsigned target = (unsigned)host_env_ms("DHD_SND_LATENCY_MS", 70, 20, 400) * per_ms;
    unsigned device = DEVICE_MS * per_ms, low = 4 * per_ms;
    int running = 0, started = 0, trace = getenv("DHD_SND_HOST_TRACE") != NULL;
    double phase = 0, last_trace = host_wall_seconds(), speed = 1.0, speed_at = last_trace;
    long speed_write = ring_write;
    unsigned trace_min = ~0u, trace_max = 0;
    static int16_t buf[CHUNK_FRAMES * 2];
    if (target < device + 10 * per_ms) target = device + 10 * per_ms;
    for (;;) {
        unsigned padding, level, queued, need, made = 0;
        SDL_Delay(2);
        padding = (unsigned)SDL_GetAudioStreamQueued(stream) / 4;  /* bytes queued in the stream: frames */
        {   /* the game's sound rate against real time, smoothed over ~0.5 s */
            double now = host_wall_seconds(), dt = now - speed_at;
            if (dt >= 0.05) {
                long w = ring_write;
                double inst = (w - speed_write) / (dt * SND_RATE), a = dt / 0.5;
                if (running) speed += (a > 1 ? 1 : a) * ((inst > 1.5 ? 1.5 : inst) - speed);
                speed_write = w; speed_at = now;
            }
        }
        level = (unsigned)(ring_write - ring_read);
        queued = padding + level;
        if (level > target + SKIP_MS * per_ms) {
            /* far behind the game (it caught up after a stall): skip to the cushion */
            long skip = (long)(queued - target);
            if (skip > ring_write - ring_read) skip = ring_write - ring_read;
            ring_read += skip;
            host_skipped += skip;
            level -= (unsigned)skip;
            queued = target;
        }
        need = device > padding ? device - padding : 0;
        if (!need) continue;
        if (need > CHUNK_FRAMES) need = CHUNK_FRAMES;
        host_histograms(queued);
        if (trace) {  /* a line a second (DHD_SND_HOST_TRACE), as sound_out_win32.c */
            double now = host_wall_seconds();
            if (queued < trace_min) trace_min = queued;
            if (queued > trace_max) trace_max = queued;
            if (now - last_trace >= 1.0) {
                hle_log("snd host (sdl): queue %u-%u ms, rate %+.0f ppm, game speed %.3f, underruns %ld, silence %.1f ms, dropped %ld, skipped %ld\n",
                        trace_min / per_ms, trace_max / per_ms, (host_ratio - 1) * 1e6, speed,
                        host_underruns, host_silence / (double)per_ms, host_dropped, host_skipped);
                trace_min = ~0u; trace_max = 0; last_trace = now;
            }
        }
        if (!running && queued >= target) running = started = 1;
        if (running) {
            /* proportional rate control: +0.5% when 50% over the cushion */
            double fill = ((double)queued - target) / target, error = fill * 0.01;
            host_ratio = 1.0 + (error > RATE_RANGE ? RATE_RANGE : error < -RATE_RANGE ? -RATE_RANGE : error);
            made = host_resample(buf, need, host_ratio, &phase);
            if (made < need && padding + made < low) {
                running = 0;  /* out of cushion: one gap, then build it again */
                host_underruns++;
            }
        }
        if (made < need && !running) {
            memset(buf + made * 2, 0, (need - made) * 4);
            if (started) host_silence += need - made;
            made = need;
        }
        host_frames += made;
        SDL_PutAudioStreamData(stream, buf, (int)(made * 4));
    }
}

static int host_thread(void *unused)
{
    SDL_AudioSpec spec;
    SDL_AudioStream *stream;
    (void)unused;
    host_thread_name("sound");
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        if (hle_verbose || trace_on())
            hle_log("989snd: SDL audio did not start (%s)\n", SDL_GetError());
        return 0;
    }
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq = SND_RATE;
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (!stream) {
        if (hle_verbose || trace_on())
            hle_log("989snd: no SDL audio device (%s)\n", SDL_GetError());
        return 0;
    }
    SDL_ResumeAudioStreamDevice(stream);
    hle_log("989snd: SDL audio output started (%d Hz stereo PCM, backend %s)\n", SND_RATE,
            SDL_GetCurrentAudioDriver() ? SDL_GetCurrentAudioDriver() : "?");
    host_loop(stream);
    SDL_DestroyAudioStream(stream);
    return 0;
}

static const char *host_bench_tag(void) { return " sdl"; }

static void host_start(void)
{
    if (!getenv("DHD_HEADLESS")) {
        SDL_Thread *thread = SDL_CreateThread(host_thread, "sound", NULL);
        if (thread) SDL_DetachThread(thread);
    }
}

void host_sound_log_stack(void)
{
    hle_log("989snd: unresolved-play stack: (no stack capture in the SDL output)\n");
}
