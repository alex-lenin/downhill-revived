/* UNIT 0x0016FBB0 0x0016FC00 */
#include "path_motion.h"
struct Flags { char pad[0x14]; unsigned short x14; };
struct Part { char pad[8]; struct Flags *x8; };
struct Obj { char pad0[0x80]; struct Part x80; char pad1[0x214-0x8c]; float x214; char pad2[0x235-0x218]; unsigned char x235; };
extern void func_00170228(struct Obj *, float);

void func_0016FBB0(struct Obj *obj) {
    struct Part *part;
    obj->x235 = 0;
    func_00170228(obj, obj->x214);
    part = &obj->x80;
    if ((part->x8->x14 >> 1) & 1) func_001DDA50((char *)obj + 0x10, (PathTrack *)part, 0);
}
