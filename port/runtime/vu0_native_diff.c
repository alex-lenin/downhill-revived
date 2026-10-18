/* Raw-bit native VU0 differential (port/tools/test_vu0_native.py). A local capture supplies
   code only; all VF/ACC/VI/flag inputs are regenerated. The original interpreter is linked
   independently and the entire CPU state is compared. Game data stays local.
   usage: vu0_native_diff CAPTURE [CASES] */
#include "ps2asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int ps2_vu0_reference(ps2_cpu *, unsigned);

static unsigned rng = 81257;
static unsigned next(void)
{
    rng = rng * 1664525u + 1013904223u;
    return rng;
}

int main(int argc, char **argv)
{
    unsigned char code[4096];
    unsigned h[4];
    ps2_cpu input, a, b;
    ps2_q128 memory[256], reference_memory[256], native_memory[256];
    FILE *f = fopen(argv[1], "rb");
    if (!f) return 2;
    if (fread(h, 16, 1, f) != 1 || h[1] != sizeof input || fread(&input, sizeof input, 1, f) != 1 ||
        fread(code, 4096, 1, f) != 1 || fread(memory, 4096, 1, f) != 1)
        return 2;
    fclose(f);
    unsigned entry = h[0];
    if (entry != 0 && entry != 0x5e8 && entry != 0x2e0 && entry != 0x7b8 && entry != 0x808) return 2;
    unsigned end = entry == 0 ? 0xc0 : entry == 0x2e0 ? 0x4f0 : entry == 0x5e8 ? 0x760 : entry == 0x7b8 ? 0x808 : 0x830;
    input.vu0_code = code;
    input.vu0_mem = memory;
    ps2_vu0_native_mode = 1;
    /* Zeros, denormals, extremes, infinities and NaN encodings, mixed into random words. */
    static const unsigned edges[] = {0, 0x80000000, 1, 0x80000001, 0x7fffff, 0x807fffff, 0x800000, 0x80800000,
                                     0x7f7fffff, 0xff7fffff, 0x7f800000, 0xff800000, 0x7fffffff, 0xffffffff,
                                     0x3f800000, 0xbf800000};
    unsigned n = argc > 2 ? atoi(argv[2]) : 1000000;
    for (unsigned t = 0; t < n; t++) {
        for (unsigned r = 0; r < 32; r++)
            for (unsigned j = 0; j < 4; j++) {
                unsigned x = next();
                input.vf[r].w[j] = ((x >> 16) & 3) ? next() : edges[x >> 28];
            }
        for (unsigned j = 0; j < 4; j++) {
            unsigned x = next();
            memcpy(&input.vacc[j], &x, 4);
        }
        for (unsigned r = 0; r < 256; r++)
            for (unsigned j = 0; j < 4; j++) {
                unsigned x = next();
                memory[r].w[j] = ((x >> 16) & 3) ? next() : edges[x >> 28];
            }
        for (unsigned r = 0; r < 16; r++) input.vi[r] = next() & 65535;
        input.vi[0] = 0;
        for (unsigned r = 0; r < 3; r++) input.vu0_ctrl[r] = next();
        memcpy(reference_memory, memory, sizeof memory);
        memcpy(native_memory, memory, sizeof memory);
        a = b = input;
        a.vu0_mem = reference_memory;
        b.vu0_mem = native_memory;
        int ra = ps2_vu0_reference(&a, entry), rb = ps2_vu0_run(&b, entry);
        a.vu0_mem = b.vu0_mem = memory;
        if (ra != rb || memcmp(&a, &b, sizeof a) || memcmp(reference_memory, native_memory, sizeof memory)) {
            unsigned *aa = (unsigned *)&a, *bb = (unsigned *)&b;
            for (unsigned i = 0; i < sizeof a / 4; i++)
                if (aa[i] != bb[i])
                    fprintf(stderr, "trial %u word %u reference=%08x native=%08x\n", t, i, aa[i], bb[i]);
            return 1;
        }
    }
    if (ps2_vu0_native_runs != n) {
        fprintf(stderr, "native coverage %llu expected %u\n", ps2_vu0_native_runs, n);
        return 1;
    }
    /* Changing any covered instruction must decline before touching state. */
    for (unsigned i = entry; i < end; i += 4) {
        unsigned old;
        memcpy(&old, code + i, 4);
        unsigned altered = old ^ 1;
        memcpy(code + i, &altered, 4);
        a = b = input;
        int ra = ps2_vu0_reference(&a, entry), rb = ps2_vu0_run(&b, entry);
        memcpy(code + i, &old, 4);
        if (ra != rb || memcmp(&a, &b, sizeof a)) {
            fprintf(stderr, "guard failed at %x\n", i);
            return 1;
        }
    }
    if (ps2_vu0_native_runs != n) {
        fprintf(stderr, "changed code accelerated\n");
        return 1;
    }
    input.vi[0] = 123;
    a = b = input;
    int ra = ps2_vu0_reference(&a, entry), rb = ps2_vu0_run(&b, entry);
    if (ra != rb || memcmp(&a, &b, sizeof a) || ps2_vu0_native_runs != n) {
        fprintf(stderr, "invalid VI0 guard failed\n");
        return 1;
    }
    printf("native VU0 %04X: %u raw-bit cases, %u changed-word guards, invalid VI0 MATCH\n", entry, n, (end - entry) / 4);
    return 0;
}
