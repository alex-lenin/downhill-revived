/* UNIT 0x00169FF0 0x0016A010 .sdata=0x29C884:0x29C888 */
#include "scalar_api.h"
struct Scalar00169FF0Obj { int x0; int x4; float x8; float xC; };
extern float D_0029C880;
float D_0029C884 = 0.75f;
void func_00169FF0(struct Scalar00169FF0Obj *obj, float value) {
    obj->x0 = 3;
    obj->x8 = D_0029C884;
    obj->xC = value;
    D_0029C880 = value;
}
