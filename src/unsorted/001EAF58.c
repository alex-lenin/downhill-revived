/* UNIT 0x001EAF58 0x001EAFD8 */
#include "object_registry.h"
#include "scene.h"
typedef SceneNode Target;
typedef struct Inner { char pad[0x30]; Target *x30; unsigned int *x34; } Inner;
typedef struct Obj { char pad[0x10]; Inner x10; } Obj;
extern void func_00210760(Inner *);

extern void func_00203590(void *, unsigned int, int, int);
extern void func_002038E0(void *, int);
void func_001EAF58(Obj *obj) {
    int local[4];
    Inner *inner = &obj->x10;
    Target *target;
    func_00210760(inner);
    target = inner->x30;
    target->xB4 = RegisterObjectShortRetail((void *)(obj), (unsigned int)(0));
    local[2] = 0;
    func_00203590(local, *inner->x34 >> 18, 0, 0);
    func_002038E0(local, 1);
}
