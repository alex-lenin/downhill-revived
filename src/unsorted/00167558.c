/* UNIT 0x00167558 0x001675D8 */
struct Entry { char pad0[0xC]; short xC; short xE; };
struct Obj { char pad0[0x10]; struct Entry entries[1]; char pad20[0x4E0]; int x500; char pad504[0x20]; float x524, x528, x52C, x530, x534, x538, x53C; };
extern char D_003F52D0[];
extern void func_00190688(void *);
#include "scene_effects.h"
void func_00167558(struct Obj *obj, int index) {
    int isR;

    isR = index[obj->entries].xC == 0x72;
    if (isR) {
        func_0020ECD0((int)obj->x534, (int)obj->x538, (int)obj->x53C, obj->x524, obj->x528, obj->x52C, obj->x530);
        obj->x500 = 0;
    }
    func_00190688(D_003F52D0);
}
