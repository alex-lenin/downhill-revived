/* Native versions of the game's VU0 microprograms (started from the EE with vcallms), decoded
   from the VU0 microcode the retail ELF uploads (two VIF MPG packets: micro 0x000-0x7FF and
   0x800-0xD4F). Each follows the microcode operation by operation, with the same constants and
   the same order of roundings, so results only differ from the PS2 where the VU's own float
   rules do (it rounds toward zero and has no denormals, infinities or NaNs).

   Flag reads (fmand/fsand) see an FMAC instruction's flags 4 cycles after it issues; the
   comments name the instruction each read actually sees. Only the registers the EE reads back
   are written: vf14/vf15 (0x978), vf10..vf12 (0x830), vi2/vi5 and vf10.x (0x0c0). */
#include "ps2asm.h"
#include "ps2float.h"

#include <math.h>
#include <stdint.h>

#define INV_TWO_PI 0.159154937f /* 0x3E22F983 */
#define TWO_PI 6.28318548f      /* 0x40C90FDB */
#define HALF_PI 1.57079637f     /* 0x3FC90FDB */
#define PI 3.14159274f          /* 0x40490FDB */
#define SIN_C3 -0.166666567f    /* 0xBE2AAAA4 */
#define SIN_C5 0.00833302550f   /* 0x3C08873E */
#define SIN_C7 -0.000198074136f /* 0xB94FB21F */
#define SIN_C9 2.60188699e-06f  /* 0x362E9C14 */

static float vmin(float a, float b) { return ps2_float_minmax(a, b, 0); }
static float vmax(float a, float b) { return ps2_float_minmax(a, b, 1); }

/* vftoi0 then vitof0: truncate toward zero, saturating at the int32 range. */
static float vtrunc(float x)
{
    double d = x;
    return (float)(d >= 2147483647.0 ? 2147483647 : d <= -2147483648.0 ? INT32_MIN : (int32_t)d);
}

/* sin(x) for x in [-pi/2, pi/2]: x + c3 x^3 + c5 x^5 + c7 x^7 + c9 x^9, summed as at 0xA60. */
static float sin_poly(float x)
{
    float x2 = x * x, x4 = x2 * x2, x8 = x4 * x4;
    float acc = (x * SIN_C3) * x2;
    acc = acc + ((x * SIN_C7) * x2) * x4;
    acc = acc + (x * SIN_C5) * x4;
    acc = acc + x * 1.0f;
    return acc + (x * SIN_C9) * x8;
}

/* 0x978 (entered at 0x980 by 0x830): vf14 = sin(vf14), vf15 = cos(vf14), all four fields. */
static void sincos_980(ps2_cpu *cpu)
{
    int c;
    for (c = 0; c < 4; c++) {
        float a = cpu->vf[14].f[c];
        float acc = a + 0.0f;            /* vaddax ACC, vf14, vf0x */
        float turns = a * INV_TWO_PI;
        /* x's sign flag comes from the vaddax, the others' from the vmuli */
        int negative = signbit(c == 0 ? acc : turns) != 0;
        float whole = vtrunc(turns), r;
        /* Reduce to [0, 2pi). x and y add 2pi to ACC before subtracting the whole turns;
           z and w add it after (vmaddw on vf14 once the vmsub is done). */
        if (c < 2 && negative)
            acc = acc + TWO_PI * 1.0f;
        r = acc - whole * TWO_PI;
        if (c >= 2 && negative)
            r = r + TWO_PI * 1.0f;
        {
            float three_half_pi = TWO_PI - HALF_PI;
            float s = vmax(vmin(r, PI - r), r - TWO_PI);           /* sin(r) = sin(s) */
            float k = vmax(r - three_half_pi, HALF_PI - r);        /* cos(r) = sin(k) */
            cpu->vf[14].f[c] = sin_poly(s);
            cpu->vf[15].f[c] = sin_poly(k);
        }
    }
}

static void sincos_978(ps2_cpu *cpu)
{
    cpu->vi[15] = 0; /* direct start: end with the E bit instead of jr vi15 */
    sincos_980(cpu);
}

/* 0x830: rotation rows vf10..vf12 from the angles in vf14.xyz (bal vi15 to the sin/cos above,
   then products of sx, sy, sz and cx, cy, cz). Only the xyz fields of the rows are written. */
static void rotation_830(ps2_cpu *cpu)
{
    float sx, sy, sz, cx, cy, cz;
    cpu->vi[15] = 0x840 / 8;
    sincos_980(cpu);
    sx = cpu->vf[14].f[0], sy = cpu->vf[14].f[1], sz = cpu->vf[14].f[2];
    cx = cpu->vf[15].f[0], cy = cpu->vf[15].f[1], cz = cpu->vf[15].f[2];
    cpu->vf[10].f[0] = cy * cz + sx * (sy * sz);
    cpu->vf[10].f[1] = sy * (sx * cz) - cy * sz;
    cpu->vf[10].f[2] = 0.0f - sy * cx;
    cpu->vf[11].f[0] = cx * sz;
    cpu->vf[11].f[1] = cz * cx;
    cpu->vf[11].f[2] = 0.0f + sx;
    cpu->vf[12].f[0] = sy * cz - sx * (sz * cy);
    cpu->vf[12].f[1] = (0.0f - sy * sz) - cy * (sx * cz);
    cpu->vf[12].f[2] = cx * cy;
}

/* 0x0c0: sphere against two boxes. The point vf5.xyz + vf4.xyz goes through the 3x3 matrices
   vf7..vf9 (box A) and vf1..vf3 (box B), then is offset by the radius vf5.w both ways. Result
   bits: vi2 = A+r negative in x/y (0x80/0x40) | B+r negative in any of xyz (2) | vf10.x after
   adding A+r's z is negative (0x80); vi5 = A-r negative in x/y | B-r negative in any of xyz. */
static void box_test_0c0(ps2_cpu *cpu)
{
    float p[3], r = cpu->vf[5].f[3];
    float a_plus[3], a_minus[3], b_plus[3], b_minus[3];
    int c, a_plus_sign = 0, a_minus_sign = 0, b_plus_sign = 0, b_minus_sign = 0;
    uint16_t vi3;
    for (c = 0; c < 3; c++)
        p[c] = cpu->vf[5].f[c] + cpu->vf[4].f[c];
    for (c = 0; c < 3; c++) {
        float a = (cpu->vf[7].f[c] * p[0] + cpu->vf[8].f[c] * p[1]) + cpu->vf[9].f[c] * p[2];
        float b = (cpu->vf[1].f[c] * p[0] + cpu->vf[2].f[c] * p[1]) + cpu->vf[3].f[c] * p[2];
        a_plus[c] = a + (0.0f + r);
        a_minus[c] = a - r * 1.0f;
        b_plus[c] = b + (0.0f + r);
        b_minus[c] = b - r * 1.0f;
        b_plus_sign |= signbit(b_plus[c]) != 0;
        b_minus_sign |= signbit(b_minus[c]) != 0;
    }
    /* MAC flag sign bits: x = 0x80, y = 0x40 (fmand with vi6 = 0xC0) */
    a_plus_sign = (signbit(a_plus[0]) ? 0x80 : 0) | (signbit(a_plus[1]) ? 0x40 : 0);
    a_minus_sign = (signbit(a_minus[0]) ? 0x80 : 0) | (signbit(a_minus[1]) ? 0x40 : 0);
    cpu->vf[10].f[0] = cpu->vf[10].f[0] + a_plus[2];
    vi3 = signbit(cpu->vf[10].f[0]) ? 0x80 : 0; /* fmand with vi7 = 0x80 */
    /* status flag S (fsand 2): any written field negative */
    cpu->vi[2] = (uint16_t)((b_plus_sign ? 2 : 0) | a_plus_sign | vi3);
    cpu->vi[5] = (uint16_t)((b_minus_sign ? 2 : 0) | a_minus_sign);
    cpu->vi[3] = vi3;
    cpu->vi[4] = (uint16_t)a_minus_sign;
}

void ps2_vu0_init(void)
{
    ps2_vu0_register_micro(0x0c0, box_test_0c0);
    ps2_vu0_register_micro(0x830, rotation_830);
    ps2_vu0_register_micro(0x978, sincos_978);
}
