/* UNIT 0x0015AC68 0x0015AD60 */
#include "npc_motion.h"
typedef struct Value { int x[4]; } Value;

typedef struct Obj {
    char pad0[0x28];
    Value x28;
    char pad38[0x1E0 - 0x38];
    short x1E0;
} Obj;

extern float func_00203C28(Value *);
extern unsigned char func_00204230(Value *);
extern void func_00202E38(Value *, int, int, float);
extern int func_00222BD8(int, int);
extern void func_00159CE0(Obj *, int);


void func_0015AC68(Obj *obj)
{
    Value a;
    Value b;
    int flag = 0;

    a = obj->x28;
    if (func_00203C28(&a) >= 1.0f || (b = obj->x28, !func_00204230(&b))) flag = 1;
    if ((unsigned char)flag) func_00202E38(&obj->x28, 1, 1, 150.0f);
    if (obj->x1E0 > 180 && func_00222BD8(0, 100) == 42) {
        func_00159CE0(obj, 1);
    }
    func_0015A968((NpcMotionActor *)(obj));
}
