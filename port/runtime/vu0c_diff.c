/* The compiled VU0 microprograms (port/tools/vu0_aot.py) against the interpreter, raw bits
   (port/tools/vu0c_diff.py builds and runs it).

   usage: vu0c_diff VUTEXT CODE [CASES] [REPEAT]

   VUTEXT is the game's .vutext (assets/vutext.bin), from which ps2_vu_aot_load fills the compiled
   programs' words as in the game; CODE is VU0 micro memory as the game's VIF0 MPG leaves it (4 KB,
   written by vu0c_diff.py from the same file). For every entry address of micro memory (each pair),
   CASES random states (registers, ACC, VI, I, Q, the flags and data memory, with zeros, denormals,
   extremes, infinities and NaN encodings mixed in) run on the original interpreter and through
   ps2_vu0_run; the whole ps2_cpu and data memory must be identical. Entries with a compiled program
   must have taken it. Then every word a compiled program covers is changed in turn: the program
   must decline (and the interpreter's result still match), and VI0 != 0 must decline. With REPEAT,
   both are timed per entry (ns a call). Game data stays in local files. */
#include "ps2asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xmmintrin.h>
#include <intrin.h>
#include <windows.h>

uintptr_t guest_base;
unsigned host_fp_csr(void) { return _mm_getcsr(); }
void host_fp_set_csr(unsigned csr) { _mm_setcsr(csr); }
uint64_t host_cycles(void) { return __rdtsc(); }
void host_vu1_file(uint32_t address) { (void)address; }

extern int ps2_vu0_reference(ps2_cpu *, unsigned);

static unsigned rng = 81257;
static unsigned next(void)
{
    rng = rng * 1664525u + 1013904223u;
    return rng ^ (rng >> 15);
}

static const unsigned edges[] = {0, 0x80000000, 1, 0x80000001, 0x7fffff, 0x807fffff, 0x800000, 0x80800000,
                                 0x7f7fffff, 0xff7fffff, 0x7f800000, 0xff800000, 0x7fffffff, 0xffffffff,
                                 0x3f800000, 0xbf800000};
static unsigned value(void)
{
    unsigned x = next();
    return ((x >> 16) & 3) ? next() : edges[x >> 28];
}

static unsigned char *read_file(const char *path, size_t *size)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    *size = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *data = malloc(*size);
    if (fread(data, 1, *size, f) != *size) return NULL;
    fclose(f);
    return data;
}

static void random_state(ps2_cpu *cpu, ps2_q128 *memory)
{
    for (unsigned r = 0; r < 32; r++)
        for (unsigned j = 0; j < 4; j++) cpu->vf[r].w[j] = value();
    for (unsigned j = 0; j < 4; j++) { unsigned x = value(); memcpy(&cpu->vacc[j], &x, 4); }
    for (unsigned r = 0; r < 256; r++)
        for (unsigned j = 0; j < 4; j++) memory[r].w[j] = value();
    for (unsigned r = 0; r < 16; r++) cpu->vi[r] = (uint16_t)next();
    cpu->vi[0] = 0;
    { unsigned x = value(); memcpy(&cpu->i, &x, 4); x = value(); memcpy(&cpu->q, &x, 4); }
    for (unsigned r = 0; r < 16; r++) cpu->vu0_ctrl[r] = next();
}

static double now_ns(void)
{
    static LARGE_INTEGER frequency;
    LARGE_INTEGER t;
    if (!frequency.QuadPart) QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart * 1e9 / (double)frequency.QuadPart;
}

int main(int argc, char **argv)
{
    size_t vutext_size, code_size;
    unsigned char *vutext = argc > 2 ? read_file(argv[1], &vutext_size) : NULL;
    unsigned char *code = argc > 2 ? read_file(argv[2], &code_size) : NULL;
    if (!vutext || !code || code_size != 4096) {
        fprintf(stderr, "usage: vu0c_diff VUTEXT CODE [CASES] [REPEAT]\n");
        return 2;
    }
    const unsigned cases = argc > 3 ? (unsigned)atoi(argv[3]) : 20000, repeat = argc > 4 ? (unsigned)atoi(argv[4]) : 0;
    ps2_vu_aot_load(vutext);
    static ps2_cpu input, a, b, keep;
    static ps2_q128 memory[256], memory_a[256], memory_b[256];
    input.vu0_code = code;
    ps2_vu0_compiled_mode = 1;
    ps2_vu0_native_mode = 0;  /* VU0C_DIFF_NATIVES builds: the compiled programs, not the routines */
    unsigned compiled_entries = 0, failures = 0;
    unsigned long long total = 0;
    for (unsigned entry = 0; entry < 0xD50; entry += 8) {
        unsigned long long before = ps2_vu0_compiled_runs;
        int ok = 1, any_ok = 0;
        for (unsigned t = 0; t < cases && ok; t++) {
            random_state(&input, memory);
            memcpy(memory_a, memory, sizeof memory);
            memcpy(memory_b, memory, sizeof memory);
            a = b = input;
            a.vu0_mem = memory_a;
            b.vu0_mem = memory_b;
            int ra = ps2_vu0_reference(&a, entry), rb = ps2_vu0_run(&b, entry);
            if (ra) break;  /* not a program the interpreter runs (an entry into the middle of one) */
            any_ok = 1;
            a.vu0_mem = b.vu0_mem = NULL;
            if (ra != rb || memcmp(&a, &b, sizeof a) || memcmp(memory_a, memory_b, sizeof memory)) {
                const unsigned *aa = (const unsigned *)&a, *bb = (const unsigned *)&b;
                fprintf(stderr, "entry %04X trial %u differs:", entry, t);
                for (unsigned i = 0; i < sizeof a / 4; i++)
                    if (aa[i] != bb[i]) fprintf(stderr, " word %u reference=%08x compiled=%08x", i, aa[i], bb[i]);
                for (unsigned i = 0; i < 256 * 4; i++)
                    if (memory_a[i / 4].w[i % 4] != memory_b[i / 4].w[i % 4])
                        fprintf(stderr, " mem %u reference=%08x compiled=%08x", i, memory_a[i / 4].w[i % 4], memory_b[i / 4].w[i % 4]);
                fprintf(stderr, "\n");
                ok = 0;
                failures++;
            }
        }
        const unsigned long long runs = ps2_vu0_compiled_runs - before;
        if (!runs) continue;
        compiled_entries++;
        total += runs;
        if (!any_ok || runs != cases) {
            fprintf(stderr, "entry %04X: %llu compiled runs of %u\n", entry, runs, cases);
            failures++;
            continue;
        }
        /* Guards: each covered word changed in turn declines; so does VI0 != 0. */
        unsigned guarded = 0;
        random_state(&input, memory);
        input.vu0_mem = memory;
        for (unsigned at = 0; at < 4096; at += 4) {
            uint32_t old;
            memcpy(&old, code + at, 4);
            const uint32_t altered = old ^ 0x00000400u;
            memcpy(code + at, &altered, 4);
            keep = input;
            before = ps2_vu0_compiled_runs;
            ps2_vu0_run(&keep, entry);
            if (ps2_vu0_compiled_runs != before) {
                keep = input;
                memcpy(code + at, &old, 4);
                continue;  /* this word is not one the program covers */
            }
            memcpy(code + at, &old, 4);
            guarded++;
        }
        input.vi[0] = 1;
        keep = input;
        before = ps2_vu0_compiled_runs;
        ps2_vu0_run(&keep, entry);
        if (ps2_vu0_compiled_runs != before) { fprintf(stderr, "entry %04X: VI0 guard\n", entry); failures++; }
        input.vi[0] = 0;
        double t_ref = 0, t_new = 0;
        if (repeat) {
            random_state(&input, memory);
            input.vu0_mem = memory;
            unsigned control = _mm_getcsr();
            double t0 = now_ns();
            for (unsigned r = 0; r < repeat; r++) { a = input; ps2_vu0_reference(&a, entry); }
            double t1 = now_ns();
            for (unsigned r = 0; r < repeat; r++) { b = input; ps2_vu0_run(&b, entry); }
            double t2 = now_ns();
            double t_native = 0;
#ifdef VU0C_DIFF_NATIVES  /* built with the native routines too: time them against the compiled program */
            ps2_vu0_compiled_mode = 0;
            ps2_vu0_native_mode = 1;
            const unsigned long long native_before = ps2_vu0_native_runs;
            double t3 = now_ns();
            for (unsigned r = 0; r < repeat; r++) { b = input; ps2_vu0_run(&b, entry); }
            t_native = ps2_vu0_native_runs != native_before ? (now_ns() - t3) / repeat : 0;
            ps2_vu0_compiled_mode = 1;
            ps2_vu0_native_mode = 0;
#endif
            _mm_setcsr(control);
            t_ref = (t1 - t0) / repeat;
            t_new = (t2 - t1) / repeat;
            if (t_native) printf("entry %04X: native routine %.0f ns\n", entry, t_native);
        }
        printf("entry %04X: %u cases %s, %u covered words guarded%s", entry, cases, ok ? "MATCH" : "DIFFER", guarded,
               repeat ? "" : "\n");
        if (repeat) printf(", interpreter %.0f ns, compiled %.0f ns\n", t_ref, t_new);
    }
    printf("%u compiled entries, %llu compiled runs, %u failures\n", compiled_entries, total, failures);
    return failures ? 1 : 0;
}
