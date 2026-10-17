/* UNIT 0x00164C38 0x00164CE0 */
#include "animation.h"
typedef AnimationController Elem;

struct Obj {
    char pad0[0x1B8];
    Elem x1B8[6];
    char pad410[0x8];
    int x418;
    char pad41C[0xC];
    int x428;
    char pad42C[0xC];
    int x438;
    char pad43C[0xC];
    int x448;
    char pad44C[0xC];
    void *vtbl;
};

extern char D_002857A0[];
extern void func_002318C0(struct Obj *obj);


struct Obj *func_00164C38(struct Obj *obj) {
    obj->vtbl = D_002857A0;
    func_002318C0(obj);
    func_001AC780((AnimationController *)(&obj->x1B8[0]), 4, 999);
    func_001AC780((AnimationController *)(&obj->x1B8[1]), 4, 999);
    func_001AC780((AnimationController *)(&obj->x1B8[2]), 4, 999);
    func_001AC780((AnimationController *)(&obj->x1B8[3]), 4, 999);
    func_001AC780((AnimationController *)(&obj->x1B8[4]), 4, 999);
    func_001AC780((AnimationController *)(&obj->x1B8[5]), 4, 999);
    obj->x418 = 0;
    obj->x428 = 0;
    obj->x438 = 0;
    obj->x448 = 0;
    return obj;
}
