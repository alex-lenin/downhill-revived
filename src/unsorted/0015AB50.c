/* UNIT 0x0015AB50 0x0015AC68 */
#include "npc_motion.h"
typedef struct Value { int x[4]; } Value;

typedef struct Obj {
    char pad0[0x78];
    Value x78;
    char pad88[0x1E0 - 0x88];
    short x1E0;
} Obj;

extern float func_00203C28(Value *);
extern unsigned char func_00204230(Value *);
extern int func_00222BD8(int, int);
extern void func_00159CE0(Obj *, int);


void func_0015AB50(Obj *obj) {
    Value a;
    Value b;
    int flag = 0;

    a = obj->x78;
    if (func_00203C28(&a) >= 1.0f || (b = obj->x78, !func_00204230(&b))) flag = 1;
    if ((unsigned char)flag && obj->x1E0 > 10) {
        if (func_00222BD8(0, 1)) {
            func_00159CE0(obj, 1);
        } else if (func_00222BD8(0, 1)) {
            func_00159CE0(obj, 4);
        } else {
            func_00159CE0(obj, 5);
        }
    }
    func_0015A968((NpcMotionActor *)(obj));
}
