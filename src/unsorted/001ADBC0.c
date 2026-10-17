/* UNIT 0x001ADBC0 0x001ADE98 */
#include "rider.h"
typedef Vector4 Vec;
typedef struct {
    int x0;
    float x4, x8;
    int xC;
    float x10;
    char pad14[0x24-0x14];
    float x24;
    char pad28[0xF8-0x28];
    Rider *xF8;
} Obj;
extern float func_00236088(float, float, float, float);
extern int func_00138560(void *);
extern void func_001ADE98(Obj *, float);
extern void func_001AE0A8(Obj *);
void func_001ADBC0(Obj *obj)
{
    Vec *vp;
    SceneNode *b;
    Vec *fwd;
    float half;
    float len, inv, dot, f;
    Vec n;
    float fz, fy, fx;
    float fwx, fwy, fwz;
    int v1, v2;
    int onebits;
    float onef;
    if (obj->xF8 == 0)
        return;
    obj->x10 = func_00236088(obj->x10, 1.0f, 0.002f, 0.001f);
    half = obj->x10 * 0.5f;
    if (func_00138560((char *)obj->xF8 + 0x40C0) & 0xFF) {
        func_001ADE98(obj, 1.0f);
        obj->x4 = func_00236088(obj->x4, 0.0f, 0.02f, 0.01f);
    } else if (obj->xF8->physics.x160 == 1) {
        vp = &obj->xF8->physics.velocity;
        __asm__ volatile("" : "=r"(onebits));
        __asm__ volatile("" : "=f"(len));
        fx = obj->xF8->physics.velocity.x;
        fz = vp->z;
        fy = vp->y;
        __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(fx), "f"(fy), "f"(fz), "r"(vp));
        onebits = 0x3F800000;
        __asm__ volatile("mtc1 %1,%0" : "=f"(onef) : "r"(onebits));
        inv = onef / len;
        n.x = fx * inv;
        n.y = fy * inv;
        n.z = fz * inv;
        b = obj->xF8->bodyNode;
        __asm__ volatile("" : "=f"(fwz));
        fwd = &b->matrix.row[1];
        fwx = b->matrix.row[1].x;
        fwy = fwd->y;
        fwz = fwd->z;
        __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(fwx),"f"(n.x),"f"(fwy),"f"(n.y),"f"(fwz),"f"(n.z));
        f = 1.0f - dot;
        func_001ADE98(obj, f);
        obj->x4 = func_00236088(obj->x4, 0.0f, 0.02f, 0.01f);
    } else {
        if (obj->xF8->x13D8)
            v1 = obj->xF8->x125C;
        else
            v1 = obj->xF8->x1194;
        if (v1 != 0) {
            obj->x8 = func_00236088(obj->x8, -half, 0.05f, 0.04f);
            obj->x4 = func_00236088(obj->x4, -half, 0.12f, 0.06f);
            obj->x24 = func_00236088(obj->x24, -half, 0.05f, 0.04f);
        } else {
            if (obj->xF8->x13D7)
                v2 = obj->xF8->x1258;
            else
                v2 = obj->xF8->x1190;
            if (v2 != 0) {
                obj->x8 = func_00236088(obj->x8, half, 0.05f, 0.04f);
                obj->x4 = func_00236088(obj->x4, half, 0.12f, 0.06f);
                obj->x24 = func_00236088(obj->x24, half, 0.05f, 0.04f);
            } else {
                obj->x4 = func_00236088(obj->x4, 0.0f, 0.02f, 0.01f);
                obj->x8 = func_00236088(obj->x8, 0.0f, 0.05f, 0.04f);
                obj->x24 = func_00236088(obj->x24, 0.0f, 0.2f, 0.1f);
            }
        }
    }
    func_001AE0A8(obj);
}
