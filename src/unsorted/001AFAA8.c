/* UNIT 0x001AFAA8 0x001AFB88 */
#include "animation.h"
typedef struct { char pad[9]; char x9; short xA; char rest[0x64-12]; } Anim;
typedef struct { char pad[0x3FA4]; int x3FA4; char pad2[0x796C-0x3FA8]; int x796C; } Rider;
typedef struct { int x0, x4; Rider *x8; Anim xC; AnimationHandle x70; Anim x80; } Obj;
extern void func_001AC7B0(Anim *, void *);
extern void func_001AC810(Anim *, float);
extern void func_001ACAE0(Anim *, float);

void func_001AFAA8(Obj *obj, Rider *rider) {
    Anim *first, *second;
    AnimationHandle *handle;
    if (rider) obj->x8 = rider;
    first = &obj->xC;
    first->xA = 0;
    handle = &obj->x70;
    second = &obj->x80;
    func_001AC7B0(first, (char *)obj->x8 + 0x2204);
    func_001AC810(first, 0.0f);
    first->x9 = 4;
    func_002036C8(handle, obj->x8->x796C + 0x15, 0, 2, obj->x8->x3FA4);
    second->xA = 0;
    func_001AC7B0(second, handle);
    func_001AC810(second, 0.0f);
    second->x9 = 4;
    func_001ACAE0(second, 0.0f);
    obj->x0 = 0;
    obj->x4 = -1;
}
