/* UNIT 0x0015A7C0 0x0015A890 */
#include "npc_motion.h"
#include "animation.h"
typedef AnimationHandle Value;
typedef struct { char pad0[0x58]; Value x58; char pad68[0x178]; short x1E0; } Obj;
extern void func_00159CE0(Obj *, int);

void func_0015A7C0(Obj *obj) {
    Value progress, status;
 /* a 1-byte struct (not a plain char) keeps the andi 0xFF truncations */
 struct { unsigned char v; } flag;
 flag.v = 0;
 if ((progress = obj->x58, func_00203C28(&progress)) >= 1.0f) flag.v = 1;
 else if (!(status = obj->x58, func_00204230(&status))) flag.v = 1;
 if (flag.v && obj->x1E0 >= 11) func_00159CE0(obj, 2);
 func_0015A968((NpcMotionActor *)(obj));
}
