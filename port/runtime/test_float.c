#include "ps2float.h"
#include "ee_native_float.h"
#include "fp_trap.h"
#include <windows.h>
#include <xmmintrin.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned failures;
static volatile float sink;
static void bits(const char *name, float value, uint32_t expected)
{
    uint32_t actual;
    memcpy(&actual, &value, 4);
    if (actual != expected) {
        fprintf(stderr, "%s: %08X != %08X\n", name, actual, expected);
        failures++;
    }
}
static float raw(uint32_t value) { volatile uint32_t input = value; float f; value = input; memcpy(&f, &value, 4); return f; }
static float num(float value) { volatile float input = value; return input; }
__declspec(noinline) float func_fp_div(float a, float b) { return a / b; }
__declspec(noinline) float host_fp_div(float a, float b) { return a / b; }
static LONG WINAPI crashed(EXCEPTION_POINTERS *e) { ExitProcess(17); return EXCEPTION_EXECUTE_HANDLER; }

int main(int argc, char **argv)
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    SetUnhandledExceptionFilter(crashed);
    ps2_fp_trap_init();
    ps2_fp_trap_enable();
    if (argc > 1 && argv[1][0] == 'r') {
        _mm_setcsr(_mm_getcsr() | 0x1F80u);
        ps2_x87_checkpoint(0x001B4128, 0);
        if (_mm_getcsr() & 0x680u) return 8;
        sink = func_fp_div(0, 0); /* a host reset must not disable the next game trap */
        return 9;
    }
    if (argc > 1 && argv[1][0] == 'g') { sink = func_fp_div(0, 0); return 9; }
    if (argc > 1 && argv[1][0] == 'h') {
        bits("host IEEE division", host_fp_div(1, 0), 0x7f800000);
        if (_mm_getcsr() & 0x680) return 8;
        sink = func_fp_div(0, 0); /* host retry must restore traps for game code */
        return 9;
    }
    _mm_setcsr(_mm_getcsr() & ~(0x680u | 63u));
    bits("positive / zero", ps2_float_div(1, 0), 0x7f7fffff);
    bits("zero / zero", ps2_float_div(0, 0), 0x7f7fffff);
    bits("negative / positive zero", ps2_float_div(-1, 0), 0xff7fffff);
    bits("positive / negative zero", ps2_float_div(1, raw(0x80000000)), 0xff7fffff);
    bits("denormal divisor", ps2_float_div(1, raw(1)), 0x7f7fffff);
    bits("negative denormal divisor", ps2_float_div(1, raw(0x80000001)), 0xff7fffff);
    bits("division overflow", ps2_float_div(raw(0x7f7fffff), 0.5f), 0x7f7fffff);
    bits("multiplication overflow", ps2_float_mul(raw(0xff7fffff), 2), 0xff7fffff);
    bits("addition overflow", ps2_float_add(raw(0x7f7fffff), raw(0x7f7fffff)), 0x7f7fffff);
    bits("subtraction overflow", ps2_float_sub(raw(0xff7fffff), raw(0x7f7fffff)), 0xff7fffff);
    bits("zero times max", ps2_float_mul(0, raw(0x7f7fffff)), 0);
    bits("exp255 input times zero", ps2_float_mul(raw(0x7fc00000), 0), 0);
    bits("negative underflow", ps2_float_mul(raw(0x80800000), 0.5f), 0x80000000);
    bits("negative sqrt", ps2_float_sqrt(-4), 0x40000000);
    bits("normal quotient", ps2_float_div(9, 4), 0x40100000);
    bits("VU zero rsqrt", ps2_vu_rsqrt(0, 0), 0);
    bits("VU signed zero rsqrt", ps2_vu_rsqrt(0, raw(0x80000000)), 0x80000000);
    bits("VU rsqrt signed divide", ps2_vu_rsqrt(1, raw(0x80000000)), 0xff7fffff);
    bits("VU MADD product overflow", ps2_vu_madd(raw(0xff7fffff), raw(0x7f7fffff), 2, 0), 0x7f7fffff);
    bits("VU MSUB product overflow", ps2_vu_madd(raw(0x7f7fffff), raw(0x7f7fffff), 2, 1), 0xff7fffff);
    failures += ps2_float_to_int(raw(0x7f7fffff)) != INT32_MAX;
    failures += ps2_float_to_int(raw(0xff7fffff)) != INT32_MIN;
    failures += ps2_float_to_int(raw(0x7fc00000)) != INT32_MAX;
    failures += ps2_float_to_int(raw(0x4effffff)) != 2147483520;
    failures += ps2_float_to_int(raw(0xcf000000)) != INT32_MIN;
    failures += ps2_float_to_int(-3.75f) != -3;
    /* EE rounding (finding 026): EE code runs toward zero with FTZ/DAZ (kernel.c ee_float_mode).
       ADD/SUB keep one guard bit, DIV and SQRT round to nearest; the VU rounds toward zero. */
    unsigned host = _mm_getcsr();
    _mm_setcsr(host | 0xE040u);
    bits("EE SUB keeps one guard bit", ps2_ee_sub(1, raw(0x33800001)), 0x3f7fffff);
    bits("chopped SUB without it", ps2_float_sub(1, raw(0x33800001)), 0x3f7ffffe);
    bits("EE SUB, smaller operand first", ps2_ee_sub(raw(0x33800001), 1), 0xbf7fffff);
    bits("EE ADD", ps2_ee_add(1, raw(0x33800001)), 0x3f800000);
    bits("EE DIV rounds to nearest", ps2_ee_div(num(1), num(3)), 0x3eaaaaab);
    bits("VU DIV rounds toward zero", ps2_float_div(num(1), num(3)), 0x3eaaaaaa);
    bits("EE DIV by zero", ps2_ee_div(num(-1), num(0)), 0xff7fffff);
    bits("EE SQRT rounds to nearest", ps2_ee_sqrt(num(5)), 0x400f1bbd);
    bits("VU SQRT rounds toward zero", ps2_float_sqrt(num(5)), 0x400f1bbc);
    bits("native SUB keeps one guard bit", ps2_native_fsub(1, raw(0x33800001)), 0x3f7fffff);
    bits("native ADD", ps2_native_fadd(1, raw(0x33800001)), 0x3f800000);
    bits("native DIV rounds to nearest", ps2_native_fdiv(num(1), num(3)), 0x3eaaaaab);
    bits("native double to float rounds to nearest", ps2_native_dtof(1.0 / num(3)), 0x3eaaaaab);
    double tenth = ps2_native_ddiv(num(1), num(10));
    uint64_t tenth_bits;
    memcpy(&tenth_bits, &tenth, 8);
    if (tenth_bits != 0x3fb999999999999aull) {  /* double arithmetic rounds to nearest */
        fprintf(stderr, "native double DIV: %016llX\n", (unsigned long long)tenth_bits);
        failures++;
    }
    failures += (_mm_getcsr() & ~63u) != ((host | 0xE040u) & ~63u);  /* the mode survives the double ops */
    _mm_setcsr(host);
    puts(failures ? "EE float tests failed" : "EE float exceptional cases pass with SSE traps enabled");
    return failures != 0;
}
