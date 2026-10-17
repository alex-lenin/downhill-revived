/* UNIT 0x00209460 0x00209590 */
#include "spatial.h"
typedef float Mtx[4][4] __attribute__((aligned(16)));
extern void func_00221BE0(Mtx, Mtx, Mtx);
extern void func_00221C20(Mtx, Mtx, float);
extern void func_00221B48(Mtx, Mtx, Mtx);

void func_00209460(SpatialMotion *obj, Vector4 *pos, Matrix4 *out, float t)
{
    Vector4 vel;
    Mtx m;
    Mtx m2;
    float s;

    s = t * obj->xB0;
    func_002092B8(obj, &vel);
    func_00221BE0(m, (float (*)[4])&obj->x40, (float (*)[4])&obj->xC0);
    func_00221C20(m, m, 1.0f / obj->xB0);
    func_002219A8((Matrix4 *)m2, (Matrix4 *)m, &obj->xC0);
    /* pos.xyz = obj->x90.xyz + vel.xyz * s */
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "mfc1 $2, %3\n\t"
        "qmtc2 $2, $vf13\n\t"
        "vaddax.xyz ACC, $vf11, $vf0x\n\t"
        "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
        "sqc2 $vf11, 0x0(%0)"
        : : "r"(pos), "r"(&obj->x90), "r"(&vel), "f"(s) : "$2", "memory");
    func_00221930((Matrix4 *)m, (Matrix4 *)m2, &obj->xC0);
    func_00221C20(m, m, s);
    func_00221B48((float (*)[4])out, (float (*)[4])&obj->xC0, m);
    pos->w = out->row[0].w = out->row[1].w = out->row[2].w =
        out->row[3].x = out->row[3].y = out->row[3].z = 0.0f;
    out->row[3].w = 1.0f;
}
