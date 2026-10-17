/* UNIT 0x0015AB08 0x0015AB50 */
#include "npc_motion.h"
struct Obj { char pad0[0x1e0]; short x1e0; char pad1e2[8]; short x1ea; };
extern void func_00159CE0(struct Obj *, int);

void func_0015AB08(struct Obj *obj) {
    if (obj->x1e0 > obj->x1ea) func_00159CE0(obj, 3);
    func_0015A968((NpcMotionActor *)(obj));
}

