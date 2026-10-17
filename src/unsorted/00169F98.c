/* UNIT 0x00169F98 0x00169FF0 .sdata=0x29C874:0x29C878 */
#include "scalar_api.h"
struct Scalar00169F98Obj { int x0, x4; float x8, xC; };
extern char D_0026EC60[];
float D_0029C874 = 0.0f;
extern float func_0016AD98(void *, float);
void func_00169F98(struct Scalar00169F98Obj *obj, float value) {
    obj->xC = value;
    obj->x8 = func_0016AD98(D_0026EC60, value);
    obj->x0 = 2;
    D_0029C874 = obj->xC;
    obj->x8 = 1.0f;
}
