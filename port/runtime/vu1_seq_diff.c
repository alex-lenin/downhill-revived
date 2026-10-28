/* Native VU1 kernels against the exact path, on captured run sequences (DHD_VU_SEQ, port/hle/vif.c).

   Every captured run starts from its real registers and data memory and runs three times: on
   the interpreter alone (the referee), through the translated blocks with the native kernels
   off, and with them on. The blocks (with their lazy MAC updates, finding 090 VU1R) and the
   kernels must each leave the whole register file (ps2_cpu), all 16 KB of data memory and every
   GIF packet it kicks (the bytes, in order, with their addresses) identical to the referee's. The code sits at the game's VU1 micro memory address
   (0x11008000) and ps2_vu1_code_version changes with it, so the block and kernel caches behave
   as in the game. Each run is then timed in both modes (DIFF_REPEAT times, state restored
   outside the timed part), per microprogram key. Inputs are local game captures; nothing here
   embeds game data.

   usage: vu1_seq_diff DIR [repeat] [max_runs] > keys.csv   (summary and any difference on stderr) */
#include "ps2asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <xmmintrin.h>
#include <intrin.h>

/* The host functions vu0_interp.c calls (host_win32.c's, in the game). Guest address 0 is host
   address 0 here: micro memory is mapped at its PS2 address (main). */
uintptr_t guest_base;
unsigned host_fp_csr(void) { return _mm_getcsr(); }
void host_fp_set_csr(unsigned csr) { _mm_setcsr(csr); }
uint64_t host_cycles(void) { return __rdtsc(); }
void host_vu1_file(uint32_t address) { (void)address; }

typedef struct { unsigned header[8]; ps2_cpu cpu; ps2_q128 data[1024]; } Run;
enum { GIF_MAX = 1 << 20 };
typedef struct { ps2_cpu cpu; ps2_q128 data[1024]; uint8_t gif[GIF_MAX]; unsigned gif_bytes, kicks; } Result;

static ps2_q128 *data;
static Result *recording;
#ifdef PS2_VU_COVERAGE
extern unsigned long long ps2_vu1_interpreted[2048], ps2_vu1_translated;
static unsigned long long translated_pairs[2], fallback_pairs[2][2048];
#endif

/* XGKICK: the GIF packet at `address` (tags and data, through the EOP tag's data) and where it was. */
static int kick(void *opaque, unsigned address)
{
    (void)opaque;
    Result *r = recording;
    if (!r) return 0;
    r->kicks++;
    if (r->gif_bytes + 4 <= GIF_MAX) { memcpy(r->gif + r->gif_bytes, &address, 4); r->gif_bytes += 4; }
    for (unsigned guard = 0; guard < 1024;) {
        const ps2_q128 *tag = &data[address & 1023];
        uint64_t t = tag->d[0];
        unsigned nloop = t & 0x7FFF, eop = t >> 15 & 1, flg = t >> 58 & 3, nreg = t >> 60 & 15;
        unsigned qw = flg == 0 ? nloop * (nreg ? nreg : 16) : flg == 1 ? (nloop * (nreg ? nreg : 16) + 1) / 2 : nloop;
        for (unsigned i = 0; i <= qw && guard < 1024; i++, address++, guard++)
            if (r->gif_bytes + 16 <= GIF_MAX) { memcpy(r->gif + r->gif_bytes, &data[address & 1023], 16); r->gif_bytes += 16; }
        if (eop) break;
    }
    return 0;
}

static uint8_t *code_base;  /* 0x11008000, as in the game */
static uint32_t loaded_hash;
static const char *dir;

static void load_code(uint32_t hash)
{
    if (hash == loaded_hash) return;
    char path[1024];
    snprintf(path, sizeof path, "%s/code-%08X.bin", dir, hash);
    FILE *f = fopen(path, "rb");
    if (!f || fread(code_base, 0x4000, 1, f) != 1) { fprintf(stderr, "missing %s\n", path); exit(2); }
    fclose(f);
    loaded_hash = hash; ps2_vu1_code_version++;
}

static ps2_q128 memory[1024];
static void execute(const Run *r, Result *out)
{
    out->cpu = r->cpu;
    out->cpu.vu0_code = code_base; out->cpu.vu0_mem = memory;
    memcpy(memory, r->data, sizeof memory);
    data = memory; recording = out; out->gif_bytes = out->kicks = 0;
#ifdef PS2_VU_COVERAGE
    if (ps2_vu1_aot_mode) {
        memset(ps2_vu1_interpreted, 0, sizeof ps2_vu1_interpreted);
        ps2_vu1_translated = 0;
    }
#endif
    if (ps2_vu1_run(&out->cpu, r->header[0], r->header[1], r->header[2], kick, NULL)) {
        fprintf(stderr, "VU failure in run with entry %04X\n", r->header[0]); exit(1);
    }
    memcpy(out->data, memory, sizeof memory);
#ifdef PS2_VU_COVERAGE
    if (ps2_vu1_aot_mode) {
        unsigned mode = ps2_vu1_native_mode == 1;
        translated_pairs[mode] += ps2_vu1_translated;
        for (unsigned i = 0; i < 2048; i++) fallback_pairs[mode][i] += ps2_vu1_interpreted[i];
    }
#endif
    recording = NULL;
}

static double timed(const Run *r, unsigned repeat)
{
    LARGE_INTEGER a, b, f;
    double total = 0;
    QueryPerformanceFrequency(&f);
    for (unsigned k = 0; k < repeat; k++) {
        ps2_cpu cpu = r->cpu;
        cpu.vu0_code = code_base; cpu.vu0_mem = memory;
        memcpy(memory, r->data, sizeof memory);
        data = memory;
        QueryPerformanceCounter(&a);
        ps2_vu1_run(&cpu, r->header[0], r->header[1], r->header[2], kick, NULL);
        QueryPerformanceCounter(&b);
        total += (double)(b.QuadPart - a.QuadPart) / f.QuadPart;
    }
    return total / repeat;
}

typedef struct { unsigned program, entry, dispatch, runs; double exact, native; } Key;
static Key keys[512];
static unsigned key_count;

static Key *key_of(const Run *r)
{
    unsigned dispatch = r->header[7] ? r->cpu.vi[13] : 0;  /* MSCNT continuations dispatch on VI13 */
    for (unsigned i = 0; i < key_count; i++)
        if (keys[i].program == r->header[6] && keys[i].entry == r->header[0] && keys[i].dispatch == dispatch) return &keys[i];
    if (key_count == 512) { fprintf(stderr, "too many keys\n"); exit(2); }
    keys[key_count] = (Key){r->header[6], r->header[0], dispatch, 0, 0, 0};
    return &keys[key_count++];
}

static long first_difference(const void *a, const void *b, size_t n)
{
    const uint8_t *x = a, *y = b;
    for (size_t i = 0; i < n; i++) if (x[i] != y[i]) return (long)i;
    return -1;
}

int main(int argc, char **argv)
{
    if (argc < 2) { fprintf(stderr, "usage: vu1_seq_diff DIR [repeat] [max_runs]\n"); return 2; }
    dir = argv[1];
    unsigned repeat = argc > 2 ? (unsigned)atoi(argv[2]) : 0, max = argc > 3 ? (unsigned)atoi(argv[3]) : ~0u;
    uint8_t *micro = VirtualAlloc((void *)(uintptr_t)0x11000000, 0x10000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (micro != (uint8_t *)(uintptr_t)0x11000000) { fprintf(stderr, "cannot map VU1 micro memory at 0x11008000\n"); return 2; }
    code_base = micro + 0x8000;
    char path[1024];
    snprintf(path, sizeof path, "%s/seq.bin", dir);
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); return 2; }
    static Run run;
    static Result referee, exact, native;
    unsigned n = 0, differing = 0;
    unsigned long long native_before[PS2_VU1_NATIVE_KINDS], coverage[PS2_VU1_NATIVE_KINDS] = {0};
    while (n < max && fread(&run, sizeof run, 1, f) == 1) {
        if (run.header[3] != sizeof(ps2_cpu)) { fprintf(stderr, "capture register size %u, this build %u\n", run.header[3], (unsigned)sizeof(ps2_cpu)); return 2; }
        /* Optional input perturbation, applied identically before all three
           executions: cover entry sticky Z/S = 00, 01, 10, 11, including the
           transition from accumulating flags to the already-set fast path. */
        if (getenv("DHD_VU_TEST_STICKY_CYCLE"))
            run.cpu.vu0_ctrl[0] = (run.cpu.vu0_ctrl[0] & ~0xc0u) | ((n & 3) << 6);
        load_code(run.header[4]);
        ps2_vu1_aot_mode = 0; ps2_vu1_native_mode = 0;
        execute(&run, &referee);
        ps2_vu1_aot_mode = 1;
        execute(&run, &exact);
        memcpy(native_before, ps2_vu1_native_runs, sizeof native_before);
        ps2_vu1_native_mode = 1;
        execute(&run, &native);
        for (unsigned i = 0; i < PS2_VU1_NATIVE_KINDS; i++) coverage[i] += ps2_vu1_native_runs[i] - native_before[i];
        const Result *checked[2] = {&exact, &native};
        int bad = 0;
        for (unsigned c = 0; c < 2; c++) {
            const Result *x = checked[c];
            long cpu = first_difference(&referee.cpu, &x->cpu, sizeof referee.cpu);
            long mem = first_difference(referee.data, x->data, sizeof referee.data);
            long gif = referee.gif_bytes != x->gif_bytes || referee.kicks != x->kicks ? 0 :
                       first_difference(referee.gif, x->gif, referee.gif_bytes);
            if (cpu < 0 && mem < 0 && gif < 0) continue;
            if (differing + bad < 20)
                fprintf(stderr, "run %u (field %u, program %04X entry %04X vi13 %04X), %s: registers %ld, data %ld (qword %ld), gif %ld (%u/%u bytes, %u/%u kicks)\n",
                        n, run.header[5], run.header[6], run.header[0], run.cpu.vi[13], c ? "blocks + kernels" : "blocks",
                        cpu, mem, mem >= 0 ? mem / 16 : -1, gif, referee.gif_bytes, x->gif_bytes, referee.kicks, x->kicks);
            bad = 1;
        }
        differing += bad;
        Key *k = key_of(&run);
        k->runs++;
        if (repeat) {
            ps2_vu1_native_mode = 0; k->exact += timed(&run, repeat);  /* the blocks, no kernels */
            ps2_vu1_native_mode = 1; k->native += timed(&run, repeat);
        }
        n++;
    }
    fclose(f);
    printf("program,entry,dispatch,runs,exact_us,native_us,speedup\n");
    double exact_total = 0, native_total = 0;
    for (unsigned i = 0; i < key_count; i++) {
        Key *k = &keys[i];
        exact_total += k->exact; native_total += k->native;
        printf("%04X,%04X,%04X,%u,%.3f,%.3f,%.3f\n", k->program, k->entry, k->dispatch, k->runs,
               k->exact * 1e6 / k->runs, k->native * 1e6 / k->runs, k->native ? k->exact / k->native : 0);
    }
    fprintf(stderr, "%u runs, %u differing; native kernel runs:", n, differing);
    for (unsigned i = 0; i < PS2_VU1_NATIVE_KINDS; i++) fprintf(stderr, " %u=%llu", 1u << i, coverage[i]);
    if (repeat) fprintf(stderr, "; replay time exact %.3f ms, native %.3f ms (%.3fx)", exact_total * 1e3, native_total * 1e3,
                        exact_total / native_total);
    fprintf(stderr, "\n");
#ifdef PS2_VU_COVERAGE
    for (unsigned mode = 0; mode < 2; mode++) {
        unsigned long long interpreted = 0;
        for (unsigned i = 0; i < 2048; i++) interpreted += fallback_pairs[mode][i];
        fprintf(stderr, "coverage %s: AOT %llu pairs, interpreter %llu pairs\n",
                mode ? "blocks+kernels" : "blocks", translated_pairs[mode], interpreted);
        for (unsigned i = 0; i < 2048; i++) if (fallback_pairs[mode][i])
            fprintf(stderr, "  fallback %04X: %llu pairs\n", i * 8, fallback_pairs[mode][i]);
    }
#endif
    return differing ? 1 : 0;
}
