/* UNIT 0x00212688 0x00212738 */
#include "spatial.h"
/* dst.xyz = matrix rotation of src.xyz, dst.w = src.w */
void func_00212688(SpatialReference *obj, const Vector4 *src, Vector4 *dst, float t)
{
    float w;
    Vector4 pos;
    Matrix4 matrix;

    w = src->w;
    if (obj->node) {
        __asm__ volatile ("lqc2 $vf11,0x0(%0)\n\tlqc2 $vf12,0x0(%1)\n\tlqc2 $vf13,0x10(%1)\n\tlqc2 $vf14,0x20(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0x0(%2)"
                          : : "r"(src), "r"(obj->node->matrix), "r"(dst) : "memory");
        dst->w = w;
    } else {
        if (obj->actor) {
        func_00209460(obj->actor, &pos, &matrix, t);
        __asm__ volatile ("lqc2 $vf11,0x0(%0)\n\tlqc2 $vf12,0x0(%1)\n\tlqc2 $vf13,0x10(%1)\n\tlqc2 $vf14,0x20(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0x0(%2)"
                          : : "r"(src), "r"(&matrix), "r"(dst) : "memory");
        }
        dst->w = w;
    }
}
