/* UNIT 0x0015B018 0x0015B100 */
#include "animation.h"
typedef AnimationHandle Value;
typedef struct Obj {
    char pad0[0x28];
    Value x28;
    char pad38[0x1E8 - 0x38];
    short x1E8;
} Obj;

extern void func_0015AE10(Obj *);

void func_0015B018(Obj *obj) {
    Value progress, status;
    /* a 1-byte struct (not a plain char) keeps the andi 0xFF truncations */
    struct { unsigned char v; } flag;

    if (--obj->x1E8 != 0) {
        flag.v = 0;
        if ((progress = obj->x28, func_00203C28(&progress)) >= 1.0f) flag.v = 1;
        else if (!(status = obj->x28, func_00204230(&status))) flag.v = 1;
        if (flag.v) func_00202E38(&obj->x28, 1, 1, 110.0f);
    } else {
        func_0015AE10(obj);
    }
}
