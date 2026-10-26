/* Host side of the 989snd output (finding 022, phase 2 part C, finding 075): the WASAPI
   device, its thread and the time-stretch stage; the ring from the mixer, the counters and the
   bench line are sound_out_common.inc's, shared with sound_out_sdl.c. The mixer
   (port/hle/sound.c) renders game time and hands its PCM over with host_sound_write; nothing
   here affects what the game can observe. Interface: port/host/host_api.h, "sound output". */
#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <math.h>

/* The class/interface ids of WASAPI, spelled out so the file needs no
   initguid TU (clang emits none of these symbols otherwise). */
static const GUID snd_clsid_enumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E}};
static const GUID snd_iid_enumerator = {0xA95664D2, 0x9614, 0x4F35, {0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6}};
static const GUID snd_iid_client = {0x1CB9AD4C, 0xDBFA, 0x4C32, {0xB1, 0x78, 0xC2, 0xF5, 0x68, 0xA7, 0x03, 0xB2}};
static const GUID snd_iid_render = {0xF294ACFC, 0x3146, 0x4483, {0xA7, 0xBF, 0xAD, 0xDC, 0xA7, 0xC2, 0x60, 0xE2}};

#include "sound_out_common.inc"

/* ---------------- host output (WASAPI) ----------------
   sound_tick (the game thread) renders game time into a ring; a host thread
   feeds WASAPI from it. Nothing here is visible to the game, so only this side
   adapts. The game renders 4004/5 samples per 59.94 Hz field, in bursts at its
   own pace; the device plays at its own clock.

   Round 30 (finding 043): the thread keeps a cushion of DHD_SND_LATENCY_MS
   (default 70) queued between the ring and the device, fills the device only
   to DEVICE_MS, and follows the drift between the game's frame clock and the
   device clock by reading the ring up to 0.5% faster or slower (dynamic rate
   control, linearly interpolated). If the cushion runs out it plays silence
   once and builds the cushion again; if far too much is queued (the game
   catching up after a stall) it skips ahead. The first version filled the
   whole 100-ms device buffer at every wake and wrote zeros whenever the ring
   was momentarily empty, so a late field became a gap: DHD_SND_LEGACY=1
   keeps that loop, for measurements only.

   When the game itself runs slower than 59.94 fields/s (heavy scenes), it
   renders less sound than the device plays, and no cushion is deep enough.
   Then the thread time-stretches: it plays the ring at the game's measured
   speed (down to half) without changing pitch, by WSOLA (waveform-similarity
   overlap-add, as SoundTouch, which PCSX2 uses, does it): 20-ms Hann
   windows at a 10-ms output hop, each placed where the input best matches
   the natural continuation of the previous one, searched over +-5 ms. At
   full speed the stretcher copies the ring unchanged; it joins and leaves
   at a window boundary, where both are the same signal. DHD_SND_STRETCH=0
   turns it off (gaps instead).

   Counters (DHD_BENCH window: one "bench snd:" line; DHD_SND_HOST_TRACE=1:
   a line per second): underruns (gaps), silence written inside gaps, frames
   dropped or skipped, queue level, and the game's tick interval. */

static volatile double host_tempo = 1.0;   /* stretch tempo (1: copied) */

/* The first output loop (rounds 7-29), kept to measure against. */
static void host_legacy(IAudioClient *client, IAudioRenderClient *render_client, UINT32 frames)
{
    int gap = 0, started = 0;
    for (;;) {
        UINT32 padding = 0, want, i;
        BYTE *dst = NULL;
        if (FAILED(IAudioClient_GetCurrentPadding(client, &padding))) break;
        want = frames > padding ? frames - padding : 0;
        if (want == 0) { Sleep(5); continue; }
        host_histograms(padding + (UINT32)(ring_write - ring_read));
        if (FAILED(IAudioRenderClient_GetBuffer(render_client, want, &dst))) break;
        for (i = 0; i < want; i++) {
            long r = ring_read;
            int16_t *out = (int16_t *)dst + i * 2;
            if (r < ring_write) {
                out[0] = ring[(r % RING_SAMPLES) * 2];
                out[1] = ring[(r % RING_SAMPLES) * 2 + 1];
                ring_read = r + 1;
                started = 1;
                gap = 0;
            } else {
                out[0] = out[1] = 0;
                if (started) {
                    if (!gap) host_underruns++;
                    gap = 1;
                    host_silence++;
                }
            }
        }
        host_frames += want;
        IAudioRenderClient_ReleaseBuffer(render_client, want, 0);
    }
}

/* The time-stretch stage: ring -> fifo, copied or WSOLA-stretched (see above). */
#define ST_HOP 480           /* 10 ms output hop; the window is twice that */
#define ST_SEEK 240          /* +-5 ms search, at most the shortest input hop */
#define ST_FIFO 8192
static int16_t st_fifo[ST_FIFO * 2];
static long st_read, st_write;
static struct {
    int on;                  /* stretching; else copying */
    long ref;                /* input position of the previous window's natural continuation */
    double where;            /* fractional input position of the next window */
    float rise[ST_HOP], tail[ST_HOP * 2];  /* the previous window's falling half, L/R */
} st;

static float st_mono(long at) { const int16_t *f = &ring[(at % RING_SAMPLES) * 2]; return (float)f[0] + f[1]; }

/* Best start in [lo, hi] for a window like the one at ref (normalized cross-correlation on the
   mono sum, every second sample, then refined by one sample each way). */
static long st_seek(long lo, long hi, long ref)
{
    static float want[ST_HOP / 2];
    long best = lo, at;
    double best_score = -1e300;
    for (int i = 0; i < ST_HOP / 2; i++) want[i] = st_mono(ref + 2 * i);
    for (int pass = 0; pass < 2; pass++) {
        long from = pass ? best - 1 : lo, to = pass ? best + 1 : hi, step = pass ? 1 : 2;
        if (from < lo) from = lo;
        if (to > hi) to = hi;
        for (at = from; at <= to; at += step) {
            double dot = 0, energy = 1;
            for (int i = 0; i < ST_HOP / 2; i++) {
                float x = st_mono(at + 2 * i);
                dot += x * want[i]; energy += x * x;
            }
            double score = dot / sqrt(energy);
            if (score > best_score) { best_score = score; best = at; }
        }
    }
    return best;
}

static void st_put(int16_t l, int16_t r)
{
    st_fifo[(st_write % ST_FIFO) * 2] = l;
    st_fifo[(st_write % ST_FIFO) * 2 + 1] = r;
    st_write++;
}

static int16_t st_clamp(float v) { return (int16_t)(v > 32767.f ? 32767 : v < -32768.f ? -32768 : lrintf(v)); }

/* Leaves stretching at the natural continuation, where copying gives the same signal. */
static void st_leave(void)
{
    if (st.on) { ring_read = st.ref; st.on = 0; }
}

/* Puts frames into the fifo until it holds `want`, at `tempo` (input frames per output frame). */
static void st_fill(UINT32 want, double tempo)
{
    if (!st.rise[ST_HOP - 1])
        for (int i = 0; i < ST_HOP; i++) {
            double w = sin(3.14159265358979 / 2 * (i + 0.5) / ST_HOP);
            st.rise[i] = (float)(w * w);
        }
    if (tempo >= 1.0) st_leave();
    else if (!st.on) {  /* join: the copied signal is the previous window */
        st.on = 1; st.ref = ring_read; st.where = ring_read - ST_HOP;
        for (int i = 0; i < ST_HOP; i++) {
            const int16_t *f = &ring[((ring_read + i) % RING_SAMPLES) * 2];
            float fall = 1.0f - st.rise[i];
            st.tail[i * 2] = f[0] * fall; st.tail[i * 2 + 1] = f[1] * fall;
        }
    }
    while ((UINT32)(st_write - st_read) < want && st_write - st_read <= ST_FIFO - ST_HOP) {
        if (!st.on) {
            long r = ring_read;
            if (r >= ring_write) return;
            st_put(ring[(r % RING_SAMPLES) * 2], ring[(r % RING_SAMPLES) * 2 + 1]);
            ring_read = r + 1;
            continue;
        }
        long q = (long)floor(st.where + ST_HOP * tempo), lo = q - ST_SEEK, hi = q + ST_SEEK;
        if (lo < ring_read) lo = ring_read;
        if (hi + 2 * ST_HOP > ring_write) hi = ring_write - 2 * ST_HOP;  /* short of input: search less */
        if (hi < lo) return;  /* not enough input yet */
        long at = st_seek(lo, hi, st.ref);
        for (int i = 0; i < ST_HOP; i++) {
            const int16_t *f = &ring[((at + i) % RING_SAMPLES) * 2], *g = &ring[((at + ST_HOP + i) % RING_SAMPLES) * 2];
            float fall = 1.0f - st.rise[i];
            st_put(st_clamp(st.tail[i * 2] + f[0] * st.rise[i]), st_clamp(st.tail[i * 2 + 1] + f[1] * st.rise[i]));
            st.tail[i * 2] = g[0] * fall; st.tail[i * 2 + 1] = g[1] * fall;
        }
        host_stretched += ST_HOP;
        st.where += ST_HOP * tempo;  /* the nominal grid; the choice only moves within +-ST_SEEK */
        st.ref = at + ST_HOP;
        /* still needed: the template at ref and the next search, which starts at least at
           where (ST_SEEK <= the shortest input hop) */
        ring_read = (long)floor(st.where) < at ? (long)floor(st.where) : at;
    }
}

/* Output frames from the stretcher's fifo at `ratio` input frames per output frame;
   returns how many it could make. */
static UINT32 host_resample(int16_t *out, UINT32 count, double ratio, double *phase)
{
    UINT32 i;
    for (i = 0; i < count; i++) {
        long r = st_read;
        double f = *phase;
        const int16_t *a, *b;
        if (st_write - r < 2) break;
        a = &st_fifo[(r % ST_FIFO) * 2];
        b = &st_fifo[((r + 1) % ST_FIFO) * 2];
        out[i * 2] = (int16_t)(a[0] + (b[0] - a[0]) * f);
        out[i * 2 + 1] = (int16_t)(a[1] + (b[1] - a[1]) * f);
        f += ratio;
        while (f >= 1.0) { f -= 1.0; r++; }
        *phase = f;
        st_read = r;
    }
    return i;
}

static void host_loop(IAudioClient *client, IAudioRenderClient *render_client, UINT32 frames, HANDLE event)
{
    const UINT32 per_ms = SND_RATE / 1000;
    UINT32 target = (UINT32)host_env_ms("DHD_SND_LATENCY_MS", 70, 20, 400) * per_ms;
    UINT32 device = DEVICE_MS * per_ms, low = 4 * per_ms;
    int running = 0, started = 0, trace = getenv("DHD_SND_HOST_TRACE") != NULL;
    const char *stretch_env = getenv("DHD_SND_STRETCH");
    int stretch = !(stretch_env && *stretch_env == '0');
    double phase = 0, last_trace = host_wall_seconds(), speed = 1.0, speed_at = last_trace;
    long speed_write = ring_write;
    UINT32 trace_min = ~0u, trace_max = 0;
    if (device > frames) device = frames;
    if (target < device + 10 * per_ms) target = device + 10 * per_ms;
    for (;;) {
        UINT32 padding = 0, level, queued, need, made = 0;
        BYTE *dst = NULL;
        if (event) WaitForSingleObject(event, 20);
        else Sleep(2);
        if (FAILED(IAudioClient_GetCurrentPadding(client, &padding))) break;
        {   /* the game's sound rate against real time, smoothed over ~0.5 s */
            double now = host_wall_seconds(), dt = now - speed_at;
            if (dt >= 0.05) {
                long w = ring_write;
                double inst = (w - speed_write) / (dt * SND_RATE), a = dt / 0.5;
                if (running) speed += (a > 1 ? 1 : a) * ((inst > 1.5 ? 1.5 : inst) - speed);
                speed_write = w; speed_at = now;
            }
        }
        level = (UINT32)(ring_write - ring_read) + (UINT32)(st_write - st_read);
        queued = padding + level;
        if (level > target + SKIP_MS * per_ms) {
            st_leave();
            host_tempo = 1.0;
            level = (UINT32)(ring_write - ring_read) + (UINT32)(st_write - st_read);
            /* far behind the game (it caught up after a stall): skip to the cushion */
            long skip = (long)(queued - target);
            if (skip > ring_write - ring_read) skip = ring_write - ring_read;
            ring_read += skip;
            host_skipped += skip;
            level -= (UINT32)skip;
            queued = target;
        }
        need = device > padding ? device - padding : 0;
        if (!need) continue;
        host_histograms(queued);
        if (trace) {
            double now = host_wall_seconds();
            if (queued < trace_min) trace_min = queued;
            if (queued > trace_max) trace_max = queued;
            if (now - last_trace >= 1.0) {
                hle_log("snd host: queue %u-%u ms, rate %+.0f ppm, game speed %.3f, tempo %.3f, stretched %.1f s, underruns %ld, silence %.1f ms, dropped %ld, skipped %ld\n",
                        trace_min / per_ms, trace_max / per_ms, (host_ratio - 1) * 1e6, speed, host_tempo,
                        host_stretched / (double)SND_RATE, host_underruns, host_silence / (double)per_ms, host_dropped, host_skipped);
                trace_min = ~0u; trace_max = 0; last_trace = now;
            }
        }
        if (!running && queued >= target) running = started = 1;
        if (FAILED(IAudioRenderClient_GetBuffer(render_client, need, &dst))) break;
        if (running) {
            /* Slower than real time (heavy scene): stretch to the game's speed, corrected by the
               cushion; join below 97% or 3/4 of the cushion, leave at full speed and a full cushion. */
            double fill = ((double)queued - target) / target, tempo = 1.0;
            int slow = stretch && (speed < 0.97 || queued < target * 3 / 4);
            if (stretch && (host_tempo < 1.0 ? !(speed > 0.99 && queued >= target) : slow)) {
                tempo = speed * (1 + 0.5 * fill);
                tempo = tempo < 0.5 ? 0.5 : tempo > 0.999 ? 0.999 : tempo;
            }
            host_tempo = tempo;
            /* proportional rate control: +0.5% when 50% over the cushion (copying only) */
            double error = tempo < 1.0 ? 0 : fill * 0.01;
            host_ratio = 1.0 + (error > RATE_RANGE ? RATE_RANGE : error < -RATE_RANGE ? -RATE_RANGE : error);
            st_fill(need + 2, tempo);
            made = host_resample((int16_t *)dst, need, host_ratio, &phase);
            if (made < need && padding + made < low) {
                running = 0; /* out of cushion: one gap, then build it again */
                host_underruns++;
                st_leave();
                host_tempo = 1.0;
            }
        }
        if (made < need && !running) {
            memset(dst + made * 4, 0, (need - made) * 4);
            if (started) host_silence += need - made;
            made = need;
        }
        host_frames += made;
        IAudioRenderClient_ReleaseBuffer(render_client, made, 0);
    }
}

static DWORD WINAPI host_thread(void *unused)
{
    IMMDeviceEnumerator *enumerator = NULL;
    IMMDevice *device = NULL;
    IAudioClient *client = NULL;
    IAudioRenderClient *render_client = NULL;
    WAVEFORMATEX format;
    HRESULT hr;
    UINT32 frames = 0;
    HANDLE event = NULL;
    int legacy = getenv("DHD_SND_LEGACY") != NULL;
    DWORD flags = AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY;
    (void)unused;
    host_thread_name("sound");
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
        return 0;
    if (FAILED(CoCreateInstance(&snd_clsid_enumerator, NULL, CLSCTX_ALL,
                                &snd_iid_enumerator, (void **)&enumerator)))
        goto done;
    hr = IMMDeviceEnumerator_GetDefaultAudioEndpoint(enumerator, eRender, eConsole, &device);
    if (FAILED(hr)) {
        if (hle_verbose || trace_on())
            hle_log("989snd: no default audio endpoint (HRESULT %08lX)\n", (unsigned long)hr);
        goto done;
    }
    if (FAILED(IMMDevice_Activate(device, &snd_iid_client, CLSCTX_ALL, NULL,
                                  (void **)&client)))
        goto done;
    memset(&format, 0, sizeof format);
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = SND_RATE;
    format.wBitsPerSample = 16;
    format.nBlockAlign = 4;
    format.nAvgBytesPerSec = SND_RATE * 4;
    if (!legacy && (event = CreateEventA(NULL, FALSE, FALSE, NULL)))
        flags |= AUDCLNT_STREAMFLAGS_EVENTCALLBACK; /* woken each device period */
    /* Let Windows convert our deterministic PCM to the endpoint mix format. */
    hr = IAudioClient_Initialize(client, AUDCLNT_SHAREMODE_SHARED, flags, 1000000, 0, &format, NULL);
    if (FAILED(hr)) {
        if (hle_verbose || trace_on())
            hle_log("989snd: WASAPI initialization failed (HRESULT %08lX)\n", (unsigned long)hr);
        goto done;
    }
    if (event && FAILED(IAudioClient_SetEventHandle(client, event)))
        goto done;
    if (FAILED(IAudioClient_GetBufferSize(client, &frames)))
        goto done;
    if (FAILED(IAudioClient_GetService(client, &snd_iid_render,
                                        (void **)&render_client)))
        goto done;
    if (FAILED(IAudioClient_Start(client)))
        goto done;
    if (hle_verbose || trace_on())
        hle_log("989snd: WASAPI output started (48000 Hz stereo PCM, %u-frame buffer%s)\n",
                frames, legacy ? ", legacy loop" : "");
    if (legacy) host_legacy(client, render_client, frames);
    else host_loop(client, render_client, frames, event);
    IAudioClient_Stop(client);
done:
    if (render_client) IAudioRenderClient_Release(render_client);
    if (client) IAudioClient_Release(client);
    if (device) IMMDevice_Release(device);
    if (enumerator) IMMDeviceEnumerator_Release(enumerator);
    if (event) CloseHandle(event);
    CoUninitialize();
    return 0;
}

static const char *host_bench_tag(void) { return getenv("DHD_SND_LEGACY") ? " legacy" : ""; }

static void host_start(void)
{
    HANDLE thread = CreateThread(NULL, 0, host_thread, NULL, 0, NULL);
    if (thread)
        CloseHandle(thread);
}

void host_sound_log_stack(void)
{
    void *frames[16]; unsigned count, i;
    count = CaptureStackBackTrace(0, 16, frames, NULL);
    hle_log("989snd: unresolved-play stack:");
    for (i = 0; i < count; i++) hle_log(" %p", frames[i]);
    hle_log("\n");
}
