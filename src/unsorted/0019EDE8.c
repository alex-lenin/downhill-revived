/* UNIT 0x0019EDE8 0x0019EE98 */
#include "timer.h"
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Obj { char pad0[0x190]; Vec x190; char pad1A0[0x2E8]; int x488; } Obj;
extern char D_00759780[], D_004BD740[];
extern void func_00238A68(int);
extern void func_00203B68(Vec *, int, float);
extern void func_002184D0(int, int, int);
extern void func_00216F48(float);
extern void func_0019EA38(Obj *, int, int);
extern void func_001C2848(void *);
void func_0019EDE8(Obj *obj) {
    Vec pos;
    func_00236C00((TimerPool *)(D_00759780));
    func_00238A68(60);
    obj->x488 = 1;
    pos = obj->x190;
    func_00203B68(&pos, 1, 1.0f);
    func_002184D0(0, 0, 0);
    func_00216F48(1.0f);
    func_0019EA38(obj, 1, 0);
    func_001C2848(D_004BD740);
}
