/* UNIT 0x00235E70 0x00235EA8 */
#include "scalar_api.h"
typedef struct Obj {
    int x0;
    char pad4[0x3910];
    char x3914;
    char pad3915[3];
    int x3918;
    int x391C;
    int x3920;
} Obj;
void func_00235E70(int value, ScalarUnsigned64 address) {
    Obj *obj = (Obj *)(int)address;
    obj->x3914 = 0;
    obj->x3918 = value;
    if (value == 0) {
        if (obj->x0 != 5) obj->x0 = 1;
        obj->x3920 = -1;
    }
}
