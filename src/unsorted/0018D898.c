/* UNIT 0x0018D898 0x0018D8A8 */
#include "scalar_api.h"
typedef struct Scalar0018D898Obj {
    char pad0[0x4];
    short x4;
    char pad6[0x2];
    int x8;
    int xC;
} Obj;

void func_0018D898(Obj *obj, short a, int b) {
    obj->xC = b;
    obj->x8 = 0;
    obj->x4 = a;
}
