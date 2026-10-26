/* Raw-bit differential check of ps2float.h's single-precision VU macro arithmetic against its
   double-precision reference (port/tools/vu_madd_diff.py builds and runs it; finding 090 SP14).

   ps2_vu_madd and ps2_vu_arith4 (ADD/SUB/MUL/MADD/MSUB, four lanes, dest mask) must give the bits
   of ps2_vu_madd_reference / ps2_float_add / ps2_float_sub / ps2_float_mul in every written lane,
   on random raw-bit operands mixed with zeros, denormals, infinities, NaN encodings, the largest
   magnitudes, products near the overflow edge and near-cancelling accumulators. On x86 every case
   runs under each MXCSR given on the command line (hex; default the EE's: toward zero, FTZ, DAZ,
   exceptions masked; and plain round to nearest, the browser's wasm). On wasm32 (node) the host's
   IEEE mode only. Exit status 1 on any difference.
       vu_madd_diff CASES SEED [MXCSR ...] */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ps2float.h"
#if defined(__x86_64__) || defined(__i386__)
#include <xmmintrin.h>
#define HAVE_CSR 1
#endif

#ifdef VU_MADD_DIFF_NEGATIVE
/* Negative control: the vector MADD without its overflow fallback must be caught. */
#define ps2_vu_arith4 negative_arith4
static int negative_arith4(int kind, const float *s, const float *t, const float *acc, int dest, float *r)
{
    ps2_f32x4 a, b, c, result;
    (void)dest;
    __builtin_memcpy(&a, s, 16); __builtin_memcpy(&b, t, 16); __builtin_memcpy(&c, acc, 16);
    a = ps2_float_input4(a); b = ps2_float_input4(b); c = ps2_float_input4(c);
    switch (kind) {
    case PS2_VU4_ADD: result = a + b; break;
    case PS2_VU4_SUB: result = a - b; break;
    case PS2_VU4_MUL: result = a * b; break;
    case PS2_VU4_MADD: result = c + ps2_float_input4(a * b); break;
    default: result = c - ps2_float_input4(a * b); break;
    }
    result = ps2_float_input4(result);
    __builtin_memcpy(r, &result, 16);
    return 1;
}
#endif

static uint64_t state;
static uint32_t next(void)
{
    state ^= state << 13; state ^= state >> 7; state ^= state << 17;
    return (uint32_t)(state >> 16);
}
static float from_bits(uint32_t u) { float f; memcpy(&f, &u, 4); return f; }

/* A raw float from one of the classes the clamps and rounding care about. */
static uint32_t value(void)
{
    uint32_t sign = next() << 31, mantissa = next() & 0x7fffff, r = next() % 16;
    switch (r) {
    case 0: return sign;                                          /* zero */
    case 1: return sign | (mantissa ? mantissa : 1);              /* denormal */
    case 2: return sign | 0x7f800000;                             /* infinity */
    case 3: return sign | 0x7f800000 | (mantissa ? mantissa : 1); /* NaN encodings */
    case 4: return sign | 0x7f7fffff;                             /* largest */
    case 5: return sign | ((254 - next() % 4) << 23) | mantissa;  /* near the top */
    case 6: return sign | ((1 + next() % 4) << 23) | mantissa;    /* near the bottom */
    case 7: return sign | ((190 + next() % 4) << 23) | mantissa;  /* products near FLT_MAX */
    case 8: return sign | ((60 + next() % 8) << 23) | mantissa;   /* products near FLT_MIN */
    case 9: return sign | ((127 + next() % 3) << 23) | (next() & 0xff); /* short mantissas: exact sums */
    case 10: case 11: case 12: return sign | ((100 + next() % 55) << 23) | mantissa; /* everyday magnitudes */
    default: return next();                                       /* raw bits */
    }
}

static long failures, cases, fallbacks;
static void report(const char *what, unsigned csr, uint32_t acc, uint32_t a, uint32_t b, uint32_t got, uint32_t want)
{
    if (++failures <= 20)
        printf("DIFF %s csr=%04x acc=%08x a=%08x b=%08x got=%08x want=%08x\n", what, csr, acc, a, b, got, want);
}

static uint32_t reference(int kind, float acc, float a, float b)
{
    float r;
    switch (kind) {
    case PS2_VU4_ADD: r = ps2_float_add(a, b); break;
    case PS2_VU4_SUB: r = ps2_float_sub(a, b); break;
    case PS2_VU4_MUL: r = ps2_float_mul(a, b); break;
    case PS2_VU4_MADD: r = ps2_vu_madd_reference(acc, a, b, 0); break;
    default: r = ps2_vu_madd_reference(acc, a, b, 1); break;
    }
    return ps2_float_bits(r);
}

static void one_case(unsigned csr)
{
    uint32_t s[4], t[4], acc[4];
    for (int c = 0; c < 4; c++) {
        s[c] = value(); t[c] = value(); acc[c] = value();
        if (next() % 4 == 0) {  /* an accumulator near -(s * t): cancellation and signed zeros */
            float p = from_bits(s[c]) * from_bits(t[c]);
            uint32_t u = ps2_float_bits(p) ^ 0x80000000u;
            acc[c] = u + (next() % 5) - 2;
        }
        if (next() % 8 == 0) t[c] = t[0];  /* broadcast forms */
    }
    for (int c = 0; c < 4; c++)
        for (int subtract = 0; subtract < 2; subtract++) {
            uint32_t got = ps2_float_bits(ps2_vu_madd(from_bits(acc[c]), from_bits(s[c]), from_bits(t[c]), subtract));
            uint32_t want = reference(subtract ? PS2_VU4_MSUB : PS2_VU4_MADD, from_bits(acc[c]), from_bits(s[c]), from_bits(t[c]));
            if (got != want) report(subtract ? "msub" : "madd", csr, acc[c], s[c], t[c], got, want);
        }
    for (int kind = PS2_VU4_ADD; kind <= PS2_VU4_MSUB; kind++) {
        int dest = 1 + next() % 15;
        uint32_t r[4] = {0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef};
        if (!ps2_vu_arith4(kind, (float *)s, (float *)t, (float *)acc, dest, (float *)r)) {
            int over = 0;
            fallbacks++;
            for (int c = 0; c < 4; c++)
                if (dest & (1 << c)) {
                    float p = ps2_float_input(from_bits(s[c])) * ps2_float_input(from_bits(t[c]));
                    over |= (ps2_float_bits(p) & 0x7fffffffu) >= 0x7f7fffffu;
                }
            if (kind < PS2_VU4_MADD || !over) report("arith4 fallback", csr, acc[0], s[0], t[0], kind, dest);
            continue;
        }
        for (int c = 0; c < 4; c++)
            if (dest & (1 << c)) {
                uint32_t want = reference(kind, from_bits(acc[c]), from_bits(s[c]), from_bits(t[c]));
                if (r[c] != want) report("arith4", csr, acc[c], s[c], t[c], r[c], want);
            }
    }
    cases++;
}

int main(int argc, char **argv)
{
    long n = argc > 1 ? atol(argv[1]) : 1000000;
    state = argc > 2 ? strtoull(argv[2], 0, 0) : 0x5eed5eedull;
    if (!state) state = 1;
#ifdef HAVE_CSR
    unsigned modes[8] = {0xffc0, 0x1f80}, count = 2;
    if (argc > 3) {
        count = 0;
        for (int i = 3; i < argc && count < 8; i++) modes[count++] = (unsigned)strtoul(argv[i], 0, 16);
    }
    unsigned saved = _mm_getcsr();
    for (unsigned m = 0; m < count; m++) {
        _mm_setcsr(modes[m]);
        for (long i = 0; i < n; i++) one_case(modes[m]);
        _mm_setcsr(saved);
        printf("csr %04x: %ld cases\n", modes[m], n);
    }
#else
    for (long i = 0; i < n; i++) one_case(0);
    printf("host IEEE mode: %ld cases\n", n);
#endif
    printf("%s: %ld cases, %ld vector fallbacks, %ld differences\n", failures ? "DIFF" : "MATCH", cases, fallbacks, failures);
    return failures != 0;
}
