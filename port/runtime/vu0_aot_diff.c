/* VU0 micro mode: the translated blocks (aot0_*, port/tools/vu1_aot.py) against the interpreter.

   Inputs are local DHD_VU0_CAPTURE files (registers, VU0 code and data memory, the VU1 register
   file VU0 reads at 0x4000; port/runtime/vu0_interp.c). Each runs from its captured state with the
   blocks off (the interpreter, the referee) and on, then again from the same state with the data
   memory floats varied by a seed (counts and indices in the low 16 bits kept) to cover other
   paths. The whole register file (ps2_cpu), the 4 KB data memory, the VU1 register file and the
   result must be identical. Each input is then timed in both modes. Nothing here embeds game data.

   usage: vu0_aot_diff [-seeds N] [-repeat N] capture.bin...   (summary and differences on stderr) */
#include "ps2asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

extern int ps2_vu0_aot_mode;
static uint8_t code[4096];
static ps2_q128 input_memory[256], memory[2][256];
static ps2_q128 vu1_file[64];
static ps2_cpu vu1_state[2];

static void vu1_from_file(ps2_cpu *v, const ps2_q128 *file)
{
    memset(v, 0, sizeof *v);
    memcpy(v->vf, file, sizeof v->vf);
    for (unsigned i = 0; i < 16; i++) v->vi[i] = (uint16_t)file[32 + i].w[0];
    for (unsigned i = 0; i < 16; i++) v->vu0_ctrl[i] = file[48 + i].w[0];
    v->r = file[52].w[0]; memcpy(&v->i, &file[53].w[0], 4);
    memcpy(&v->q, &file[54].w[0], 4); memcpy(&v->p, &file[55].w[0], 4);
}

static double now(void)
{
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t); QueryPerformanceFrequency(&f);
    return (double)t.QuadPart / (double)f.QuadPart;
}

int main(int argc, char **argv)
{
    unsigned seeds = 8, repeat = 200, inputs = 0, differing = 0, failed = 0;
    double seconds[2] = {0, 0};
    int first = 1;
    while (first + 1 < argc && argv[first][0] == '-') {
        if (!strcmp(argv[first], "-seeds")) seeds = (unsigned)atoi(argv[first + 1]);
        else if (!strcmp(argv[first], "-repeat")) repeat = (unsigned)atoi(argv[first + 1]);
        first += 2;
    }
    for (int a = first; a < argc; a++) {
        ps2_cpu input; unsigned header[4];
        FILE *f = fopen(argv[a], "rb");
        if (!f || fread(header, sizeof header, 1, f) != 1 || header[1] != sizeof input || header[2] != sizeof vu1_file ||
            fread(&input, sizeof input, 1, f) != 1 || fread(code, sizeof code, 1, f) != 1 ||
            fread(input_memory, sizeof input_memory, 1, f) != 1 || fread(vu1_file, sizeof vu1_file, 1, f) != 1) {
            fprintf(stderr, "bad capture %s\n", argv[a]); return 2;
        }
        fclose(f);
        unsigned address = header[0], random = 0x9E3779B9u ^ address;
        for (unsigned seed = 0; seed <= seeds; seed++) {
            ps2_q128 start[256];
            memcpy(start, input_memory, sizeof start);
            if (seed)  /* floats varied, low 16 bits kept */
                for (unsigned i = 0; i < 1024; i++) {
                    random = random * 1664525u + 1013904223u;
                    uint32_t *w = &start[i / 4].w[i % 4];
                    if ((*w & 0x7F800000u) && (*w & 0x7F800000u) != 0x7F800000u)
                        *w = (*w & 0xFFFFu) | ((random >> 8) & 0x007F0000u) | (*w & 0xFF800000u);
                }
            ps2_cpu out[2]; int result[2];
            for (int mode = 0; mode < 2; mode++) {
                out[mode] = input;
                memcpy(memory[mode], start, sizeof start);
                out[mode].vu0_code = code; out[mode].vu0_mem = memory[mode];
                vu1_from_file(&vu1_state[mode], vu1_file);
                ps2_vu1_regs = &vu1_state[mode];
                ps2_vu0_aot_mode = mode;
                result[mode] = ps2_vu0_run(&out[mode], address);
                out[mode].vu0_code = NULL; out[mode].vu0_mem = NULL;
            }
            inputs++;
            if (result[0] || result[1]) failed++;
            if (result[0] != result[1] || memcmp(&out[0], &out[1], sizeof out[0]) ||
                memcmp(memory[0], memory[1], sizeof memory[0]) || memcmp(&vu1_state[0], &vu1_state[1], sizeof vu1_state[0])) {
                if (differing++ < 16) fprintf(stderr, "DIFFERENT: %s seed %u (entry %04X)\n", argv[a], seed, address);
            }
            if (seed) continue;
            for (int mode = 0; mode < 2; mode++) {  /* timing: the captured input, repeated */
                ps2_vu0_aot_mode = mode;
                double t = now();
                for (unsigned r = 0; r < repeat; r++) {
                    ps2_cpu c = input;
                    memcpy(memory[mode], start, sizeof start);
                    c.vu0_code = code; c.vu0_mem = memory[mode];
                    vu1_from_file(&vu1_state[mode], vu1_file);
                    ps2_vu1_regs = &vu1_state[mode];
                    ps2_vu0_run(&c, address);
                }
                seconds[mode] += now() - t;
            }
        }
    }
    fprintf(stderr, "vu0 aot diff: %u runs (%d captures x %u), %u differing, %u failed; interpreter %.3f s, "
            "blocks %.3f s (%.2fx) over %u repeats each\n", inputs, argc - first, seeds + 1, differing, failed,
            seconds[0], seconds[1], seconds[1] > 0 ? seconds[0] / seconds[1] : 0, repeat);
    return differing != 0;
}
