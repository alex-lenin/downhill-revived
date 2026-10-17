/* UNIT 0x00174840 0x00174908 */
#include "timer.h"
typedef struct { char pad0[0x48B0]; int x48B0; char pad1[0x124]; float x49D8; char pad2[0x31C8]; int x7BA4; } Owner;
typedef struct { Owner *owner; int x4; unsigned char x8; char pad0[3]; int xC; char pad1[0x10]; unsigned char x20; char pad2[3]; int x24; unsigned char x28, x29, x2A, x2B, x2C; } Object;
extern void func_001ACFA0(void *, int);
extern void func_00136030(void *, int);
extern void func_00261D40(void);
extern char D_00759780[];
void func_00174840(Object *obj)
{
    Owner *owner = obj->owner;
    obj->xC = 1;
    obj->x20 = 0;
    obj->x24 = 0;
    obj->x28 = 0; obj->x29 = 0; obj->x2A = 0; obj->x2B = 0; obj->x2C = 0;
    func_001ACFA0((char *)owner + 0x4580, 0);
    owner->x49D8 = 0.45f;
    owner->x48B0 = 0;
    if (owner->x7BA4 == 0) func_00136030((char *)owner + 0x40C0, 1);
    else obj->x2C = 1;
    if (!obj->x8) {
        func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00261D40), obj, 1);
        obj->x8 = 1U;
    }
}
