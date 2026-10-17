/* UNIT 0x001AEA30 0x001AEB20 */
#include "rider.h"
typedef Vector4 Vec4;

typedef SceneNode Target;

typedef Rider World;

typedef RiderComponent Level;

extern Vec4 D_007684B0[];




void func_001AEA30(Vec4 *out, Level *level)
{
    Vec4 tmp;
    Target *target;
    Vec4 *a;
    Vec4 *b;
    unsigned int idx;
    float len;
    float inv;

    target = level->node;
    idx = func_001B1C08(level) + 12;
    b = &D_007684B0[idx];
    a = &target->position;
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(tmp) : "r"(a), "r"(b));
    func_00221888(&level->rider->bodyNode->matrix, &tmp, out);

    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(out->x), "f"(out->y), "f"(out->z), "f"(out->z) : "$2");
    out->x *= inv;
    out->y *= inv;
    out->z *= inv;

    tmp.x = 0.0f;
    tmp.y = 0.0f;
    tmp.z = -1.0f;
    func_001AE8A8(level->rider->bodyNode, out, &tmp, (Matrix4 *)(out + 2));
}
