#ifndef DHD_PS2FLOAT_H
#define DHD_PS2FLOAT_H
/* EE arithmetic exceptional values, independently implemented from the
   behavior documented by PCSX2 FPU.cpp. Bitwise loads/moves stay untouched.
   Evaluate in double to avoid raising a host overflow before saturation. */
#include <stdint.h>

/* MIN/MAX select an operand's bits, including exponent-255 encodings and
   denormals. They do not perform arithmetic clamping. Order sign/magnitude
   encodings with integer keys so host NaN comparisons never run. */
static inline float ps2_float_minmax(float a, float b, int maximum)
{
    uint32_t x, y;
    __builtin_memcpy(&x, &a, 4); __builtin_memcpy(&y, &b, 4);
    uint32_t kx = (x & 0x80000000u) ? ~x : x ^ 0x80000000u;
    uint32_t ky = (y & 0x80000000u) ? ~y : y ^ 0x80000000u;
    uint32_t selected = maximum ? (kx > ky ? x : y) : (kx < ky ? x : y);
    __builtin_memcpy(&a, &selected, 4);
    return a;
}

static inline float ps2_float_input(float value)
{
    uint32_t bits;
    __builtin_memcpy(&bits, &value, 4);
    if (!(bits & 0x7f800000u)) bits &= 0x80000000u;
    else if ((bits & 0x7f800000u) == 0x7f800000u)
        bits = (bits & 0x80000000u) | 0x7f7fffffu;
    __builtin_memcpy(&value, &bits, 4);
    return value;
}

static inline float ps2_float_result(double value)
{
    if (value > 3.40282346638528859812e38) return 3.40282346638528859812e38f;
    if (value < -3.40282346638528859812e38) return -3.40282346638528859812e38f;
    return ps2_float_input((float)value);
}

static inline float ps2_float_add(float a, float b)
{ return ps2_float_result((double)ps2_float_input(a) + (double)ps2_float_input(b)); }
static inline float ps2_float_sub(float a, float b)
{ return ps2_float_result((double)ps2_float_input(a) - (double)ps2_float_input(b)); }
static inline float ps2_float_mul(float a, float b)
{ return ps2_float_result((double)ps2_float_input(a) * (double)ps2_float_input(b)); }
static inline float ps2_float_div(float a, float b)
{
    a = ps2_float_input(a); b = ps2_float_input(b);
    uint32_t x, y;
    __builtin_memcpy(&x, &a, 4); __builtin_memcpy(&y, &b, 4);
    if (!(y & 0x7fffffffu)) {
        x = ((x ^ y) & 0x80000000u) | 0x7f7fffffu;
        __builtin_memcpy(&a, &x, 4);
        return a;
    }
    return ps2_float_result((double)a / (double)b);
}
static inline float ps2_float_sqrt(float a)
{ return ps2_float_input(__builtin_sqrtf(__builtin_fabsf(ps2_float_input(a)))); }

static inline int32_t ps2_float_to_int(float a)
{
    a = ps2_float_input(a);
    if (a >= 2147483648.0f) return INT32_MAX;
    if (a <= -2147483648.0f) return INT32_MIN;
    return (int32_t)a;
}

/* The EE FPU as PCSX2 runs it (pcsx2/x86/iFPU.cpp, v2.8.2; independently implemented). EE code runs
   with results rounded toward zero (kernel.c ee_float_mode), which MUL, RSQRT and CVT.S.W follow.
   ADD and SUB keep a single guard bit: the smaller operand first loses the mantissa bits that
   alignment would shift out past it (FPU_ADD_SUB). DIV rounds to nearest (FPUDivFPCR) and so does
   SQRT. */
static inline void ps2_ee_align(float *a, float *b)
{
    uint32_t x, y;
    __builtin_memcpy(&x, a, 4); __builtin_memcpy(&y, b, 4);
    int shift = (int)((x >> 23) & 255) - (int)((y >> 23) & 255);
    if (shift >= 25) y &= 0x80000000u;
    else if (shift > 0) y &= 0xffffffffu << (shift - 1);
    else if (shift <= -25) x &= 0x80000000u;
    else if (shift < 0) x &= 0xffffffffu << (-shift - 1);
    __builtin_memcpy(a, &x, 4); __builtin_memcpy(b, &y, 4);
}

/* A double rounded to the nearest float (ties to even) by its bits, whatever MXCSR's mode: the
   low 29 mantissa bits are rounded off first, so the conversion is exact. A float quotient or
   square root, computed in double, is never close enough to a halfway point for the double
   rounding to matter. */
static inline float ps2_float_nearest(double value)
{
    uint64_t bits;
    __builtin_memcpy(&bits, &value, 8);
    bits += 0x0fffffffu + ((bits >> 29) & 1);
    bits &= ~(uint64_t)0x1fffffff;
    __builtin_memcpy(&value, &bits, 8);
    return (float)value;
}

static inline float ps2_ee_add(float a, float b) { ps2_ee_align(&a, &b); return ps2_float_add(a, b); }
static inline float ps2_ee_sub(float a, float b) { ps2_ee_align(&a, &b); return ps2_float_sub(a, b); }
static inline float ps2_ee_div(float a, float b)
{
    uint32_t y;
    b = ps2_float_input(b);
    __builtin_memcpy(&y, &b, 4);
    if (!(y & 0x7fffffffu))
        return ps2_float_div(a, b);
    a = ps2_float_input(a);
    double q = (double)a / (double)b;
    if (q > 3.40282346638528859812e38) return 3.40282346638528859812e38f;
    if (q < -3.40282346638528859812e38) return -3.40282346638528859812e38f;
    return ps2_float_input(ps2_float_nearest(q));
}
static inline float ps2_ee_sqrt(float a)
{ return ps2_float_input(ps2_float_nearest(__builtin_sqrt((double)__builtin_fabsf(ps2_float_input(a))))); }

/* VU RSQRT has a distinct zero/zero result, unlike EE DIV. */
static inline float ps2_vu_rsqrt(float a, float b)
{
    a = ps2_float_input(a); b = ps2_float_input(b);
    uint32_t x, y;
    __builtin_memcpy(&x, &a, 4); __builtin_memcpy(&y, &b, 4);
    if (!(y & 0x7fffffffu)) {
        x = ((x ^ y) & 0x80000000u) | ((x & 0x7fffffffu) ? 0x7f7fffffu : 0);
        __builtin_memcpy(&a, &x, 4);
        return a;
    }
    return ps2_float_div(a, ps2_float_sqrt(b));
}

/* VU FMAC clamps its final result. An overflowing single-precision product
   must keep its sign even when the accumulator has the opposite sign. */
static inline float ps2_vu_madd(float acc, float a, float b, int subtract)
{
    double product = (double)ps2_float_input(a) * (double)ps2_float_input(b);
    if (product > 3.40282346638528859812e38 || product < -3.40282346638528859812e38)
        return ps2_float_result(subtract ? -product : product);
    float p = ps2_float_input((float)product);
    return subtract ? ps2_float_sub(acc, p) : ps2_float_add(acc, p);
}

#endif
