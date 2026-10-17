/* UNIT 0x001964A8 0x00196558 */
#include "object_registry.h"
#include "scene.h"
typedef SceneNode Inner;
typedef struct Obj { int x0, x4; Inner *x8; int xC, x10; } Obj;
extern void func_00211280(Inner *);

void func_001964A8(Obj *obj, int value, float *pos, int flags) {
    if (flags) {
        obj->x8->position.x = pos[0];
        obj->x8->position.y = pos[1];
        obj->x8->position.z = pos[2];
        *(int *)&obj->x8->flags = flags;
        *((char *)&obj->x8->flags + 4) = 1;
        obj->x8->flags |= (SceneNodeFlags)0x8000 << 42;
        obj->xC = 0;
        obj->x10 = 0;
        obj->x4 = value;
        func_00211280(obj->x8);
        if (!obj->x8->xB4) obj->x8->xB4 = RegisterObjectShortRetail((void *)(obj), (unsigned int)(0));
    }
}
