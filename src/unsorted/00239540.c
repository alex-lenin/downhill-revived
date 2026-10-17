/* UNIT 0x00239540 0x002396C8 */
#include "scalar_api.h"
/* Uses inline asm only for EE FPU and VU operations.
 * The three field addresses are created out, destination, basis in source;
 * the scheduler then emits them in retail order basis, out, destination.
 * The VU math asm is split after vwaitq so the basis and out live ranges
 * give basis the first free register, as in retail.
 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Scalar00239540Obj {
    char pad0[0x2630]; float x2630[4];
    char pad2640[0x50]; float x2690[4], x26A0[4];
    char pad26B0[0x16C]; unsigned int x281C[1];
    char pad2820[0x14]; float *x2834;
    char pad2838[0x22]; short x285A;
} Obj;
typedef union Temp { float f[4]; unsigned int u[4]; unsigned char bytes[16]; } Temp;

static inline float Dot(float *direction, float *axis)
{
    float dot;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
        : "=f"(dot) : "f"(direction[0]),"f"(axis[0]),"f"(direction[1]),"f"(axis[1]),"f"(direction[2]),"f"(axis[2]));
    return dot;
}

static inline void Scale(float *out, float *axis, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 16(%0)"
        : : "r"(out),"r"(axis),"f"(scale) : "$2","memory");
}

static inline void BuildVector(Temp *temp, float *out, float *pos,
                               float *direction, float *other, float *basis)
{
    __asm__ volatile ("lqc2 $vf10, 0(%0)" : : "r"(pos) : "memory");
    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tlqc2 $vf13, 0(%2)"
        : : "r"(direction), "r"(other), "r"(basis) : "memory");
    __asm__ volatile ("vopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tlqc2 $vf14, %0\n\tvmul.xyz $vf15, $vf11, $vf11\n\tvaddx.w $vf11, $vf14, $vf0x\n\tvadday.x ACC, $vf15, $vf15y\n\tvmaddz.x $vf15, $vf14, $vf15z\n\tvmul.xyz $vf12, $vf11, $vf13\n\tvrsqrt Q, $vf0w, $vf15x\n\tvmuly.xyzw $vf11, $vf11, $vf14y\n\tvmulax.w ACC, $vf0, $vf12x\n\tvmadday.w ACC, $vf0, $vf12y\n\tvmaddz.w $vf10, $vf0, $vf12z\n\tvwaitq"
        : "+m"(*temp) : : "memory");
    __asm__ volatile ("sqc2 $vf10, 0(%1)\n\tvmulq.xyz $vf11, $vf11, Q\n\tvftoi4.xyzw $vf11, $vf11\n\tsqc2 $vf11, %0"
        : "+m"(*temp) : "r"(out) : "memory");
}

void func_00239540(Obj *p, float *pos, float *direction, float *axis,
                   float *other, unsigned char alpha, int enabled)
{
    float dot, radius = axis[3], scale;
    Temp temp __attribute__((aligned(16)));
    float *basis, *outp;
    dot = Dot(direction, axis);
    p->x285A = (radius > 0.0024444445f) && (dot > radius * 0.5f) &&
               (alpha != 0) && (enabled != 0);
    *(u128 *)p->x2630 = *(u128 *)pos;
    if (p->x285A) {
        scale = 1.0f / radius;
        Scale(p->x2834, axis, scale);
        temp.f[0] = 1.0f;
        temp.f[1] = 1.03125f;
        temp.f[3] = (float)alpha * 0.003921569f;
        /* Reassigning axis in two steps keeps the packed destination address. */
        outp = (float *)((char *)p + 0x2690);
        axis = (float *)p;
        axis = (float *)((char *)axis + 0x281C);
        basis = (float *)((char *)p + 0x26A0);
        BuildVector(&temp, outp, pos, direction, other, basis);
        *(unsigned int *)axis = temp.bytes[0] | ((temp.u[1] << 8) & 0xFF00) |
                       ((temp.u[2] << 16) & 0xFF0000) | (temp.u[3] << 24);
    }
}
