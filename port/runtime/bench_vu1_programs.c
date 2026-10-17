/* Per-capture VU1 replay timing. Inputs are local, uncommitted game captures. */
#include "ps2asm.h"
#include "../gs/gs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct { unsigned header[4]; ps2_cpu cpu; uint32_t code[4096]; ps2_q128 data[1024]; } Capture;
static ps2_cpu cpu;
static ps2_q128 data[1024];
static uint64_t gif_ticks, kick_count;
static FILE *packet_file;
static void save_bytes(FILE *f, const void *p, size_t n)
{
    if (fwrite(p, 1, n, f) != n) { perror("snapshot write"); exit(2); }
}
static int kick(void *opaque, unsigned address)
{
    (void)opaque;
    LARGE_INTEGER a, b;
    QueryPerformanceCounter(&a);
    int result;
    if (!packet_file) result = gs_xgkick(&gs, data[0].w, address);
    else {
        save_bytes(packet_file, &address, sizeof address);
        result = -1;
        for (unsigned n = 0; n < 0x4000; n++) {
            const uint32_t *words = data[(address + n) & 1023].w;
            save_bytes(packet_file, words, 16);
            gs_feed(&gs, 1, words, 4);
            if (!gs.path[0].left && gs.path[0].eop) { result = 0; break; }
        }
    }
    QueryPerformanceCounter(&b);
    gif_ticks += b.QuadPart - a.QuadPart; kick_count++;
    return result;
}
static uint32_t code_hash(const void *ptr, unsigned n)
{
    const unsigned char *p = ptr; uint32_t h = 2166136261u;
    for (unsigned i = 0; i < n; i++) h = (h ^ p[i]) * 16777619u;
    return h;
}
int main(int argc, char **argv)
{
    if (argc < 3) { fprintf(stderr, "usage: bench repetitions capture...\n"); return 2; }
    unsigned reps = (unsigned)strtoul(argv[1], NULL, 10);
    if (!reps) return 2;
    const char *snapshot = getenv("DHD_VU1_SNAPSHOT");
    if (snapshot && reps != 1) { fprintf(stderr, "snapshots require one repeat\n"); return 2; }
    void *mapped = VirtualAlloc((void *)0x11000000, 0x10000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (mapped != (void *)0x11000000) { fprintf(stderr, "VU1 address unavailable\n"); return 2; }
    uint32_t *code = (uint32_t *)((char *)mapped + 0x8000);
    LARGE_INTEGER frequency; QueryPerformanceFrequency(&frequency);
    puts("capture,code_hash,address,repeats,vu_us,gif_us,kicks_per_call");
    for (int file = 2; file < argc; file++) {
        Capture cap;
        FILE *f = fopen(argv[file], "rb");
        if (!f) { perror(argv[file]); return 2; }
        int ok = fread(&cap, sizeof cap, 1, f) == 1 && fgetc(f) == EOF && cap.header[3] == sizeof cpu;
        fclose(f);
        if (!ok) { fprintf(stderr, "invalid capture: %s\n", argv[file]); return 2; }
        memcpy(code, cap.code, sizeof cap.code); ps2_vu1_code_version++;
        gs_reset(&gs); gif_ticks = kick_count = 0;
        char path[MAX_PATH];
        if (snapshot) {
            snprintf(path, sizeof path, "%s/%03d.gif", snapshot, file - 2);
            packet_file = fopen(path, "wb");
            if (!packet_file) { perror(path); return 2; }
        }
        uint64_t elapsed = 0;
        for (unsigned r = 0; r < reps; r++) {
            cpu = cap.cpu; memcpy(data, cap.data, sizeof data);
            cpu.vu0_code = (uint8_t *)code; cpu.vu0_mem = data;
            LARGE_INTEGER a, b; QueryPerformanceCounter(&a);
            int result = ps2_vu1_run(&cpu, cap.header[0], cap.header[1], cap.header[2], kick, NULL);
            QueryPerformanceCounter(&b); elapsed += b.QuadPart - a.QuadPart;
            if (result) { fprintf(stderr, "VU failure: %s\n", argv[file]); return 1; }
        }
        if (snapshot) {
            if (fclose(packet_file)) return 2;
            packet_file = NULL;
            snprintf(path, sizeof path, "%s/%03d.state", snapshot, file - 2);
            FILE *state = fopen(path, "wb");
            if (!state) { perror(path); return 2; }
            /* Normalize only host pointers; every guest register remains exact. */
            cpu.vu0_code = NULL; cpu.vu0_mem = NULL;
            save_bytes(state, &cpu, sizeof cpu);
            save_bytes(state, data, sizeof data);
            save_bytes(state, &gs, sizeof gs);
            if (fclose(state)) return 2;
        }
        double scale = 1e6 / frequency.QuadPart / reps;
        printf("%s,%08X,%04X,%u,%.6f,%.6f,%.3f\n", argv[file], code_hash(cap.code, sizeof cap.code),
               cap.header[0], reps, (elapsed - gif_ticks) * scale, gif_ticks * scale, (double)kick_count / reps);
        fflush(stdout);
#ifdef PS2_VU_COVERAGE
        {
            extern unsigned long long ps2_vu1_interpreted[2048], ps2_vu1_translated;
            unsigned long long interpreted = 0;
            for (unsigned pc = 0; pc < 2048; pc++) interpreted += ps2_vu1_interpreted[pc];
            fprintf(stderr, "%s: %.1f pairs a run in blocks, %.1f interpreted:", argv[file],
                    (double)ps2_vu1_translated / reps, (double)interpreted / reps);
            for (unsigned shown = 0; shown < 12; shown++) {
                unsigned best = 0;
                for (unsigned pc = 1; pc < 2048; pc++) if (ps2_vu1_interpreted[pc] > ps2_vu1_interpreted[best]) best = pc;
                if (!ps2_vu1_interpreted[best]) break;
                fprintf(stderr, " %04X:%.1f", best * 8, (double)ps2_vu1_interpreted[best] / reps);
                ps2_vu1_interpreted[best] = 0;
            }
            fprintf(stderr, "\n");
            memset(ps2_vu1_interpreted, 0, sizeof ps2_vu1_interpreted); ps2_vu1_translated = 0;
        }
#endif
    }
    return 0;
}
