/* UNIT 0x001B3AE0 0x001B3BE0 */
#include "camera.h"
#include "rider.h"
typedef SceneNode Target;

typedef Rider World;

typedef RiderComponent Owner;

typedef struct Obj {
    char pad0[0xC0];
    Owner *xC0;
    char padC4[0x10C - 0xC4];
    float x10C;
    char pad110[0x59C - 0x110];
    float x59C;
} Obj;

typedef struct View {
    float pad0[8];
    float x20[3];
    char pad2C[0x380 - 0x2C];
} View;

extern "C" char D_0037EA60[];


static inline float Dot(const float *a, const float *b)
{
    float result;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
        : "=f"(result)
        : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    return result;
}

static inline float clamp(float a)
{
    return (a >? -1.0f) <? 1.0f;
}

extern "C" void func_001B3AE0(Obj *obj)
{
    World *world;
    Target *target;
    float dot, factor, value;
    world = obj->xC0->rider;
    target = world->bodyNode;
    if (world->x7984) {
        View *view = (View *)(D_0037EA60 + world->x7988 * 0x380);
        float *axis = view->x20;
        dot = Dot(axis, &target->matrix.row[1].x);
    } else {
        View *view = (View *)D_0037EA60;
        float *axis = view->x20;
        dot = Dot(axis, &target->matrix.row[1].x);
    }
    dot = clamp(dot * 5.0f);
    factor = obj->x59C;
    value = obj->x10C * factor + dot * (1.0f - factor);
    obj->x10C = value;
    func_001B3028((RiderCamera *)(obj), 9, value);
}
