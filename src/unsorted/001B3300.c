/* UNIT 0x001B3300 0x001B33A8 */
#include "rider.h"


struct Obj { char pad[0xC0]; RiderComponent *xC0; };
extern float D_00768570[][4];



static inline void SubVector(float *out, void *a, void *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%2)"
        : : "r"(a), "r"(b), "r"(out) : "memory");
}

void func_001B3300(struct Obj *obj)
{
    float diff[4] __attribute__((aligned(16)));
    char *matrix;
    char *pos;

    matrix = (char *)obj->xC0->rider->bodyNode;
    pos = matrix + 0x10;
    SubVector(diff, &obj->xC0->node->position, pos);
    matrix += 0x40;
    func_00221888((Matrix4 *)matrix, (Vector4 *)diff, (Vector4 *)((char *)obj + 0x10));
    SubVector(diff, D_00768570[func_001B1C08(obj->xC0)], pos);
    func_00221888((Matrix4 *)matrix, (Vector4 *)diff, (Vector4 *)((char *)obj + 0x40));
}
