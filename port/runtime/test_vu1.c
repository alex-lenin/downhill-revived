/* Synthetic, redistributable VU1 programs. No retail microcode is embedded. */
#include "ps2asm.h"
#include "ps2float.h"
#include "../gs/gs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xmmintrin.h>

static ps2_cpu cpu;
static ps2_q128 data[1024];
static uint32_t code[4096];
static unsigned failures, kicks;
static unsigned trace_count;
#define CHECK(c) do { if (!(c)) { printf("FAIL VU1 line %d: %s\n", __LINE__, #c); failures++; } } while (0)
#define NOP 0x000002FFu
#define LNOP 0x8000033Cu
static uint32_t special(unsigned op, unsigned dest, unsigned ft, unsigned fs)
{
    return 0x8000003C | (op & 3) | (op >> 2) << 6 | dest << 21 | ft << 16 | fs << 11;
}
static void pair(unsigned pc, uint32_t lower, uint32_t upper)
{
    code[(pc & 2047) * 2] = lower; code[(pc & 2047) * 2 + 1] = upper;
}
static void reset(void)
{
    memset(&cpu, 0, sizeof cpu); memset(data, 0, sizeof data);
    for (unsigned i = 0; i < 2048; i++) pair(i, LNOP, NOP);
    cpu.vu0_code = (uint8_t *)code; cpu.vu0_mem = data; kicks = 0;
    gs_reset(&gs);
}
static int kick(void *arg, unsigned address)
{
    CHECK(arg == &cpu && address == 1023); kicks++;
    return gs_xgkick(&gs, data[0].w, address);
}
static void kick_trace(unsigned pc, unsigned clip, unsigned address)
{
    CHECK(pc == 600*8 && clip == 0x654321 && address == 1023);
    trace_count++;
}
static int replay_kick(void *arg, unsigned address)
{
    (void)arg;
    return gs_xgkick(&gs, data[0].w, address);
}
static int replay(const char *path)
{
    unsigned header[4];
    FILE *f = fopen(path, "rb");
    if (!f) return 1;
    if (fread(header, sizeof header, 1, f) != 1 || header[3] != sizeof cpu ||
        fread(&cpu, sizeof cpu, 1, f) != 1 || fread(code, sizeof code, 1, f) != 1 ||
        fread(data, sizeof data, 1, f) != 1) { fclose(f); return 1; }
    fclose(f); cpu.vu0_code = (uint8_t *)code; cpu.vu0_mem = data;
    int result = ps2_vu1_run(&cpu, header[0], header[1], header[2], replay_kick, NULL);
    printf("VU1 replay %04X: result %d, %llu GIF tags, %llu kicks\n", header[0], result, gs.packets[0], gs.kicks);
    return result != 0;
}
static int run(unsigned pc)
{
    return ps2_vu1_run(&cpu, pc * 8, 777, 888, kick, &cpu);
}

/* FMAC results on raw operand bits against the reference clamp: exponent-255 and denormal
   operands clamped, the operation under the VU rounding (toward zero, FTZ, DAZ), the result
   clamped again. The interpreter skips that last clamp as an identity (operand4). */
static uint32_t clamp_bits(uint32_t bits)
{
    if (!(bits & 0x7f800000)) return bits & 0x80000000;
    if ((bits & 0x7f800000) == 0x7f800000) return (bits & 0x80000000) | 0x7f7fffff;
    return bits;
}
static float clamped(uint32_t bits) { bits = clamp_bits(bits); float f; memcpy(&f, &bits, 4); return f; }
static uint32_t result_bits(float f) { uint32_t bits; memcpy(&bits, &f, 4); return clamp_bits(bits); }
static uint32_t fuzz_state = 0x2545F491;
static uint32_t fuzz_bits(void)
{
    fuzz_state ^= fuzz_state << 13; fuzz_state ^= fuzz_state >> 17; fuzz_state ^= fuzz_state << 5;
    uint32_t r = fuzz_state, sign = r & 0x80000000u, mantissa = r * 2654435761u & 0x7fffff;
    switch (r >> 8 & 7) {
    case 0: return sign | mantissa;                                   /* denormal */
    case 1: return sign | 0x7f800000 | mantissa;                      /* infinity, NaN */
    case 2: return sign | (0xF0u + (r >> 12 & 15)) << 23 | mantissa;  /* overflows */
    case 3: return sign | (1u + (r >> 12 & 15)) << 23 | mantissa;     /* underflows */
    case 4: return sign | (r & 1 ? 0x3f800000 : 0);                   /* one, zero */
    default: return r * 2246822519u;
    }
}
static void fmac_operands(void)
{
    enum { ADD = 0x28, SUB = 0x2c, MUL = 0x2a, MADD = 0x29, MSUB = 0x2d, MULI = 0x1e, ADDW = 0x03, MULW = 0x1b };
    static const unsigned ops[] = {ADD, SUB, MUL, MADD, MSUB, MULI, ADDW, MULW};
    unsigned wrong = 0;
    for (unsigned trial = 0; trial < 20000; trial++) {
        reset();
        for (unsigned c = 0; c < 4; c++)
            cpu.vf[1].w[c] = fuzz_bits(), cpu.vf[2].w[c] = fuzz_bits(), memcpy(&cpu.vacc[c], &(uint32_t){fuzz_bits()}, 4);
        memcpy(&cpu.i, &(uint32_t){fuzz_bits()}, 4);
        for (unsigned k = 0; k < 8; k++)
            pair(k, LNOP, (15u << 21) | (ops[k] == ADDW || ops[k] == MULW ? 0 : 2u << 16) | (1u << 11) | ((3 + k) << 6) | ops[k]);
        pair(8, LNOP, NOP | 1u << 30);
        ps2_cpu before = cpu;
        CHECK(run(0) == 0);
        unsigned control = _mm_getcsr();
        _mm_setcsr((control & ~0x603Fu) | 0xFFC0u);
        for (unsigned k = 0; k < 8; k++)
            for (unsigned c = 0; c < 4; c++) {
                uint32_t acc; memcpy(&acc, &before.vacc[c], 4);
                uint32_t i; memcpy(&i, &before.i, 4);
                volatile float a = clamped(before.vf[1].w[c]), b = clamped(before.vf[2].w[c]), m = clamped(acc);
                volatile float product = a * b, r;
                switch (ops[k]) {
                case ADD: r = a + b; break;
                case SUB: r = a - b; break;
                case MUL: r = product; break;
                case MADD: r = m + product; break;
                case MSUB: r = m - product; break;
                case MULI: { volatile float s = clamped(i); r = a * s; break; }
                case ADDW: { volatile float one = 1.0f; r = a + one; break; }
                default: { volatile float one = 1.0f; r = a * one; break; }
                }
                uint32_t expected = result_bits(r);
                if (cpu.vf[3 + k].w[c] != expected && wrong++ < 4)
                    printf("FMAC %02X lane %u: %08X %08X acc %08X I %08X gave %08X expected %08X\n", ops[k], c,
                           before.vf[1].w[c], before.vf[2].w[c], acc, i, cpu.vf[3 + k].w[c], expected);
            }
        _mm_setcsr(control);
    }
    CHECK(wrong == 0);
}

int main(void)
{
    if (getenv("DHD_VU_REPLAY")) return replay(getenv("DHD_VU_REPLAY"));
    reset();
    /* Full-sized data/code memories and program counter wrap, including E delay. */
    cpu.vi[1] = 1023; data[1023].w[0] = 0xDEADBEEF; data[0].w[0] = 0x12345678;
    pair(2046, (15u << 21) | (2u << 16) | (1u << 11), NOP); /* LQ vf2, 0(vi1) */
    pair(2047, special(0x34, 15, 3, 1), NOP | 1u << 30); /* LQI vf3,(vi1++) */
    pair(0, special(0x34, 15, 4, 1), NOP);
    CHECK(run(2046) == 0);
    CHECK(cpu.vf[2].w[0] == 0xDEADBEEF && cpu.vf[3].w[0] == 0xDEADBEEF && cpu.vf[4].w[0] == 0x12345678);
    CHECK(cpu.vi[1] == 1025 && cpu.vu0_ctrl[10] == 8);
    /* MSCNT starts after the E delay slot, not at the original MSCAL address. */
    pair(1, special(0x68, 0, 5, 0), NOP | 1u << 30);
    pair(2, special(0x69, 0, 6, 0), NOP);
    CHECK(run(cpu.vu0_ctrl[10] / 8) == 0 && cpu.vi[5] == 777 && cpu.vi[6] == 888);

    reset();
    data[1023].w[0] = 0x8001; data[1023].w[1] = 1u << 28; data[1023].w[2] = 14;
    data[0].w[0] = 0x11223344; data[0].w[2] = GS_FOGCOL;
    cpu.vi[2] = 1023;
    cpu.vu0_ctrl[2] = 0x654321;
    ps2_vu1_set_kick_trace(kick_trace);
    pair(600, special(0x6C, 0, 0, 2), NOP | 1u << 30);
    CHECK(run(600) == 0 && kicks == 1 && gs.reg[GS_FOGCOL] == 0x11223344);
    CHECK(trace_count == 1 && cpu.vu0_ctrl[2] == 0x654321);
    ps2_vu1_set_kick_trace(NULL);

    reset();
    cpu.p = 7; cpu.vf[1].f[0] = 4;
    pair(0, special(0x7A, 0, 0, 1), NOP);  /* ERCPR P,vf1.x */
    pair(1, special(0x64, 15, 2, 0), NOP); /* old P while EFU runs */
    pair(2, special(0x7B, 0, 0, 0), NOP); /* WAITP */
    pair(3, special(0x64, 15, 3, 0), NOP | 1u << 30);
    CHECK(run(0) == 0 && cpu.vf[2].f[0] == 7 && cpu.vf[3].w[0] == 0x3E800000);
    static const unsigned efu[] = {0x70,0x71,0x72,0x73,0x76,0x78,0x79,0x7A};
    static const uint32_t expected[] = {0x41C80000,0x3D23D70A,0x40A00000,0x3E4CCCCC,0x41100000,
                                      0x3FDDB3D7,0x3F13CD3A,0x3EAAAAAA};
    for (unsigned i = 0; i < sizeof efu / sizeof *efu; i++) {
        reset(); cpu.vf[1].f[0] = 3; cpu.vf[1].f[1] = 4; cpu.vf[1].f[3] = 2;
        pair(0, special(efu[i], 0, 0, 1), NOP);
        pair(1, special(0x7B, 0, 0, 0), NOP);
        pair(2, special(0x64, 1, 2, 0), NOP | 1u << 30);
        CHECK(run(0) == 0);
        if (cpu.vf[2].w[3] != expected[i]) printf("EFU %02X got %08X expected %08X\n", efu[i], cpu.vf[2].w[3], expected[i]);
        CHECK(cpu.vf[2].w[3] == expected[i]);
    }

    reset();
    /* IBEQ immediately after an integer write sees its old value; the branch
       delay executes and the instruction skipped by the branch does not. */
    pair(0, (8u << 25) | (1u << 16) | 1, NOP); /* IADDIU vi1,vi0,1 */
    pair(1, (0x28u << 25) | (1u << 11) | 2, NOP); /* IBEQ vi0,vi1,+2 */
    pair(2, (8u << 25) | (2u << 16) | 2, NOP);
    pair(3, (8u << 25) | (3u << 16) | 3, NOP);
    pair(4, LNOP, NOP | 1u << 30);
    CHECK(run(0) == 0 && cpu.vi[2] == 2 && cpu.vi[3] == 0);
    reset();
    /* WAITQ stalls the entire pair before the upper reads Q. */
    cpu.q = 7; cpu.vf[1].f[0] = 3;
    pair(0, special(0x38, 3, 1, 0), NOP); /* DIV Q,vf0.w,vf1.x */
    pair(1, special(0x3b, 0, 0, 0), (8u << 21) | (2u << 6) | 0x20); /* ADDq vf2.x,vf0.x,Q */
    pair(2, LNOP, NOP | 1u << 30);
    unsigned control = _mm_getcsr();
    CHECK(run(0) == 0 && cpu.vf[2].w[0] == 0x3eaaaaaa);
    CHECK(_mm_getcsr() == control);

    reset();
    /* FCSET is delayed, and unrelated arithmetic must not restore stale CLIP. */
    pair(0, 0x22012345, NOP);
    pair(1, 0x38020000, NOP); /* FCGET vi2 */
    pair(4, 0x38030000, NOP | 1u << 30);
    CHECK(run(0) == 0 && cpu.vi[2] == 0 && cpu.vi[3] == 0x345);
    CHECK(cpu.vu0_ctrl[2] == 0x12345);

    reset();
    /* Exponent-255 operands are finite VU values, not host NaNs. */
    cpu.vf[1].w[0] = 0x7fc00000; cpu.vf[1].w[1] = 0xffc00000;
    cpu.vf[1].w[2] = 0x7fffffff; cpu.vf[1].w[3] = 0xffffffff;
    pair(0, LNOP, (15u << 21) | (1u << 11) | (2u << 6) | 0x28); /* ADD vf2,vf1,vf0 */
    pair(1, LNOP, (15u << 21) | (1u << 11) | (3u << 6) | 0x2a); /* MUL vf3,vf1,vf0 */
    pair(2, LNOP, NOP | 1u << 30);
    CHECK(run(0) == 0 && cpu.vf[2].w[0] == 0x7f7fffff && cpu.vf[2].w[1] == 0xff7fffff);
    CHECK(cpu.vf[3].w[0] == 0 && cpu.vf[3].w[1] == 0x80000000 && cpu.vf[3].w[3] == 0xff7fffff);
    reset();
    cpu.vf[1].w[0] = 0x7fc00000; cpu.vf[1].w[1] = 0xffc00000;
    /* FTOI0 clamps signed NaNs by exponent; MIN/MAX compare raw bit order. */
    pair(0, LNOP, (15u << 21) | (2u << 16) | (1u << 11) | (5u << 6) | 0x3c);
    pair(1, LNOP, (15u << 21) | (1u << 11) | (3u << 6) | 0x2b);
    pair(2, LNOP, (15u << 21) | (1u << 11) | (4u << 6) | 0x2f | 1u << 30);
    CHECK(run(0) == 0 && cpu.vf[2].w[0] == 0x7fffffff && cpu.vf[2].w[1] == 0x80000000);
    CHECK(cpu.vf[3].w[0] == 0x7fc00000 && cpu.vf[4].w[1] == 0xffc00000);
    /* The SSE lanes order exactly like the scalar helper the EE and VU0 macro use. */
    static const uint32_t order[] = {0x7f800000, 0xff800000, 0x7fffffff, 0xffffffff, 0x7f7fffff,
                                     0xff7fffff, 0x3f800000, 0xbf800000, 0, 0x80000000, 1, 0x80000001};
    for (unsigned i = 0; i < 12; i++)
        for (unsigned j = 0; j < 12; j += 4) {
            reset();
            for (unsigned c = 0; c < 4; c++) { cpu.vf[1].w[c] = order[i]; cpu.vf[2].w[c] = order[j + c]; }
            pair(0, LNOP, (15u << 21) | (2u << 16) | (1u << 11) | (3u << 6) | 0x2b); /* MAX vf3,vf1,vf2 */
            pair(1, LNOP, (15u << 21) | (2u << 16) | (1u << 11) | (4u << 6) | 0x2f | 1u << 30); /* MINI vf4 */
            CHECK(run(0) == 0);
            for (unsigned c = 0; c < 4; c++) {
                float hi = ps2_float_minmax(cpu.vf[1].f[c], cpu.vf[2].f[c], 1);
                float lo = ps2_float_minmax(cpu.vf[1].f[c], cpu.vf[2].f[c], 0);
                CHECK(memcmp(&hi, &cpu.vf[3].w[c], 4) == 0 && memcmp(&lo, &cpu.vf[4].w[c], 4) == 0);
            }
        }
    fmac_operands();
    reset();
    cpu.vf[1].w[0] = 0x80000000;
    pair(0, special(0x3a, 0, 1, 0), NOP); /* RSQRT Q,+0,-0 */
    pair(1, special(0x3b, 0, 0, 0), NOP | 1u << 30);
    CHECK(run(0) == 0);
    uint32_t qbits; memcpy(&qbits, &cpu.q, 4);
    CHECK(qbits == 0x80000000 && (cpu.vu0_ctrl[0] & 0x30) == 0x30);

    printf("VU1 instruction, timing, continuation and XGKICK tests: %u failures\n", failures);
    return failures != 0;
}
