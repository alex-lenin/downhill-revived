/* UNIT 0x00239038 0x002390D0 */
#include "particle.h"
typedef unsigned int u128 __attribute__((mode(TI)));
struct Obj { char pad0[0x26A0]; float x26A0, x26A4, x26A8; char pad26AC[0x2850-0x26AC]; int x2850, x2854; short x2858, x285A; };
extern struct Obj *func_0022DD88(int);
extern float D_0027EB10[];
struct Obj *func_00239038(void) {
    u128 pos = 0;
    struct Obj *obj;
    obj = func_0022DD88(func_0022CDC0((Vector4 *)(&pos), 13, (Matrix4 *)(0), (int)(0), 1.0f, (Vector4 *)(0)));
    if (obj) {
        obj->x2854 = 0x100;
        obj->x285A = 0;
        obj->x2850 = 0;
        obj->x26A0 = D_0027EB10[0] * 0.484375f;
        obj->x26A4 = D_0027EB10[4] * 0.484375f;
        obj->x26A8 = D_0027EB10[8] * 0.484375f;
    }
    return obj;
}
