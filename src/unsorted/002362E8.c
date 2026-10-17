/* UNIT 0x002362E8 0x00236430 */
#include "rider_geometry.h"
typedef Vector4 Vec;

extern float *func_00263BA0(Vec *);


#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%2)\n\tsq $2, %0" : "=m"(dst) : "r"(&(dst)), "r"(src) : "$2")

float func_002362E8(float *pos, Vec *target, Vec *dir, Vector4 *arg3)
{
    Vec norm, delta, result;
    float len, inv, proj, distance, radius, limit, clamped, t;

    /* 1 / |dir| */
    __asm__ volatile ("mula.s %2, %3\n\tmadda.s %4, %5\n\tmadd.s %1, %6, %7\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len)
                      : "f"(dir->x), "f"(dir->x), "f"(dir->y), "f"(dir->y), "f"(dir->z), "f"(dir->z)
                      : "$2");
    norm.x = dir->x * inv;
    norm.y = dir->y * inv;
    norm.z = dir->z * inv;
    /* proj = norm . pos */
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(proj)
                      : "f"(norm.x), "f"(pos[0]), "f"(norm.y), "f"(pos[1]), "f"(norm.z), "f"(pos[2]));
    /* delta = target - pos */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(delta) : "r"(target), "r"(pos));
    /* distance = |delta| */
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                      : "=&f"(distance) : "f"(delta.x), "f"(delta.y), "f"(delta.z));
    radius = pos[3];
    limit = distance / radius * pos[7];
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(clamped) : "f"(proj), "f"(limit));
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(t) : "f"(clamped), "f"(0.0f));
    /* delta = pos + dir * t */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(delta) : "r"(pos), "r"(dir), "f"(t) : "$2");
    COPY_VECTOR(result, &delta);
    *func_00263BA0(&result) = radius;
    func_00236260((Vector4 *)(&result), (Vector4 *)(target), (Vector4 *)(dir), (Vector4 *)(arg3));
    return distance;
}
