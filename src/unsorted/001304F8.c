/* UNIT 0x001304F8 0x001306C0 .sdata=0x29C2AC:0x29C2B8 */
#include "vector.h"
typedef Vector4 Vec4;
typedef Matrix4 Mtx;

float D_0029C2AC = 0.2f;
float D_0029C2B0 = 0.25f;


/* Keep EABI aggregate parameters here: explicit copies change retail scheduling. */
int func_001304F8(Mtx *out, Mtx *in, void *m, Vec4 a, Vec4 b, Vec4 c, Vec4 *dir)
{
    Vec4 diff;
    Vec4 v;
    float dot;
    float len;
    float inv;
    float limit;
    int result;

    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(diff) : "r"(&a), "r"(&b));
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(dot) : "f"(c.x), "f"(diff.x), "f"(c.y), "f"(diff.y), "f"(c.z), "f"(diff.z));
    if (dot > 0.1f) {
        return 0;
    }

    func_00221888(m, &diff, &v);
    /* length + reciprocal; the unused 4th input (z again) is needed for the retail register allocation */
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(v.x), "f"(v.y), "f"(v.z), "f"(v.z) : "$2");
    dir->x = v.x * inv;
    dir->y = v.y * inv;
    dir->z = v.z * inv;
    if (len < D_0029C2B0) {
        return 0;
    }

    func_002226C8(in, dir);
    /* rotate rows 1 and 2 by the angle with sin 0.3 */
    __asm__ volatile ("qmfc2.ni $2, $vf11\n\tmfc1 $2, %0\n\tqmtc2.ni $2, $vf11" : : "f"(0.9539392f) : "$2");
    __asm__ volatile ("qmfc2.ni $2, $vf12\n\tmfc1 $2, %0\n\tqmtc2.ni $2, $vf12" : : "f"(0.3f) : "$2");
    __asm__ volatile ("lqc2 $vf13, 0(%0)" : : "r"(&in->row[1]));
    __asm__ volatile ("lqc2 $vf14, 0(%0)" : : "r"(&in->row[2]));
    __asm__ volatile ("vmulax.xyz ACC, $vf13, $vf11x\n\tvmaddx.xyz $vf15, $vf14, $vf12x\n\t"
                      "vmulax.xyz ACC, $vf14, $vf11x\n\tvmsubx.xyz $vf16, $vf13, $vf12x");
    __asm__ volatile ("sqc2 $vf15, 0(%0)" : : "r"(&out->row[1]) : "memory");
    __asm__ volatile ("sqc2 $vf16, 0(%0)" : : "r"(&out->row[2]) : "memory");
    out->row[0] = in->row[0];
    out->row[3] = in->row[3];

    limit = D_0029C2AC;
    result = 1;
    dir->w = dir->x * dir->x + dir->z * dir->z;
    if (!(dir->w > limit * limit)) {
        result = 0;
    }
    return result;
}
