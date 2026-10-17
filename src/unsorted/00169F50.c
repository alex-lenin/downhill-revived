/* UNIT 0x00169F50 0x00169F98 .sdata=0x29C85C:0x29C860 */
#include "scalar_api.h"
struct Scalar00169F50Obj { int x0; int x4; float x8; float xC; };
float D_0029C85C = 0.0f;
extern char D_0026EBD0[];
extern float func_0016AD98(void *, float);
void func_00169F50(struct Scalar00169F50Obj *obj, float value) {
    obj->xC = value;
    obj->x8 = func_0016AD98(D_0026EBD0, value);
    obj->x0 = 1;
    D_0029C85C = obj->xC;
}


