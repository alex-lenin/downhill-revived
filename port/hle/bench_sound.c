/* Synthetic mixer benchmark. No retail assets or audio endpoint required.
   Compile sound.c with link.py's normal 32-bit/O2/SSE2 HLE flags, including
   the host code. This executable supplies only disc/log fixtures; output is
   disabled. An optional separate O1 measurement is not the production build.
   GetThreadTimes measures CPU consumption only; it never advances the mixer.
   Reuse the redistributable test bank/stream builders to avoid a second format.
   Run from ignored build/port/audio-*; optional PCM output is synthetic. */
#define main sound_fixture_main
#include "test_sound.c"
#undef main
#include <windows.h>
#include <inttypes.h>

static uint64_t cpu_time(void)
{
    FILETIME creation, exit_time, kernel, user;
    if (!GetThreadTimes(GetCurrentThread(), &creation, &exit_time, &kernel, &user)) exit(2);
    return ((uint64_t)kernel.dwHighDateTime << 32 | kernel.dwLowDateTime) +
           ((uint64_t)user.dwHighDateTime << 32 | user.dwLowDateTime);
}

static void setup(int voice_count, int stream_count, int wet)
{
    static uint8_t bank[2048]; /* bank parser retains these bytes during rendering */
    uint8_t nibble[28];
    uint32_t request[48 / 4] = {0};
    uint32_t effect[2] = {3,3}, depth[3] = {3,16383,16383};
    uint32_t stream[8] = {1,2,512u << 16,0,1,0,0,4};
    int i, handle, size;
    snd_test_reset();
    snd_test_stream_read_samples(0); /* resident load; preparation is excluded */
    if (voice_count == 22) {
        uint32_t range[3] = {15,13,47}; /* put half the bank voices on each core */
        rpc(0x4E, range, sizeof range);
    }
    size = build_bank(bank);
    for (i = 0; i < 28; i++) nibble[i] = i % 9 - 4;
    block(bank + TEST_WAVE, 0x13, 3, nibble);
    if (wet) bank[0x18 + 0x54 + 12] = 1;
    handle = snd_test_add_bank(bank, size);
    request[0] = handle; request[2] = 512; request[3] = 0;
    memcpy(request + 4, "TEST_ONE", 8);
    for (i = 0; i < voice_count; i++) {
        /* Fast attack with the long sustain rates seen in retail bank tones
           and streams. Unlike the minimal parser fixture, ADSR is nonzero. */
        put32(bank + 0x18 + 0x54 + 18, i & 1 ? 0x9FC000FFu : 0x1FC0080Fu);
        request[2] = 128 + (i % 3) * 16;
        request[3] = (i * 137) % 360;
        request[8] = (i % 12) * 128; /* varied pitches exercise different decode rates */
        CHECK(rpc(0x5C, request, sizeof request) != 0);
    }
    make_vag(disc + 2048, 44100, 1); make_vag(disc + 4096, 44100, 2);
    for (i = 0; i < stream_count; i++) CHECK(rpc(0x2C, stream, sizeof stream) != 0);
    if (wet) { rpc(0x0E, effect, sizeof effect); rpc(0x0F, depth, sizeof depth); }
    CHECK(snd_test_active_voices() == voice_count + stream_count * 2);
    if (failures) exit(3);
}

int main(int argc, char **argv)
{
    static int16_t pcm[48000 * 2];
    static const struct { const char *name; int voices, streams, wet; } cases[] = {
        {"idle",0,0,0}, {"music",0,1,0}, {"eight_stereo_streams",0,8,0},
        {"twelve_bank_voices",12,1,0}, {"full_bank_voices",48,1,0},
        {"race_mix_wet",12,1,1}, {"twenty_four_voices_wet",24,1,1},
        {"forty_eight_voices_wet",48,1,1}, {"full_mix_wet",48,8,1},
        {"24_active_voices_reverb",22,1,1}, {"48_active_voices_reverb",46,1,1}
    };
    int seconds = argc > 1 ? atoi(argv[1]) : 30, trials = argc > 2 ? atoi(argv[2]) : 3;
    FILE *dump = argc > 3 ? fopen(argv[3], "wb") : NULL;
    unsigned c;
    int trial, second;
    const char *selected = getenv("DHD_SND_BENCH_CASE"), *cpu = getenv("DHD_SND_BENCH_CPU");
    if (cpu) {
        unsigned index = (unsigned)atoi(cpu);
        if (index >= sizeof(DWORD_PTR) * 8 ||
            !SetThreadAffinityMask(GetCurrentThread(), (DWORD_PTR)1 << index)) return 2;
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    }
    if (seconds < 1 || trials < 1 || (argc > 3 && !dump)) return 2;
    debug_env("DHD_SND_WAV", ""); debug_env("DHD_SND_REPORT", "");
    debug_env("DHD_SND_GAIN_TRACE", ""); debug_env("DHD_SND_TRACE", "");
    debug_env("DHD_SND_RNG_TRACE", ""); debug_env("DHD_SND_OFF", "1");
    debug_env("DHD_SND_GRAIN_TRACE", "");
    puts("case,trial,audio_seconds,cpu_seconds,cpu_ms_per_audio_second,final_checksum");
    for (c = 0; c < sizeof cases / sizeof *cases; c++) for (trial = 0; trial < trials; trial++) {
        uint64_t start, elapsed, checksum = 1469598103934665603ULL;
        if (selected && *selected && strcmp(selected, cases[c].name)) continue;
        setup(cases[c].voices,cases[c].streams,cases[c].wet);
        start = cpu_time();
        for (second = 0; second < seconds; second++) {
            snd_test_render(pcm,48000);
            CHECK(snd_test_active_voices() == cases[c].voices + cases[c].streams * 2);
            if (dump) fwrite(pcm,sizeof pcm,1,dump); /* evidence runs are not timing runs */
        }
        elapsed = cpu_time() - start;
        for (second = 0; second < 96000; second++)
            checksum = (checksum ^ (uint16_t)pcm[second]) * 1099511628211ULL;
        printf("%s,%d,%d,%.7f,%.4f,0x%016" PRIx64 "\n",cases[c].name,trial,seconds,
               elapsed / 10000000.0,elapsed / (seconds * 10000.0),checksum);
        fflush(stdout);
        if (failures) return 3;
    }
    snd_test_reset(); if (dump) fclose(dump);
    return 0;
}
