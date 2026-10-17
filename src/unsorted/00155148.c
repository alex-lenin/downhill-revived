/* UNIT 0x00155148 0x00155208 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001554B8_6_func_001554B8 func_001554B8
#else
#define CallScalarBridge001554B8_6_func_001554B8 ScalarBridge001554B8_6
#endif
#include "rider_geometry.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

struct Src { char pad0[0x10]; Vec x10; };

struct World {
    char pad0[0x40C0];
    char x40C0[0x4EF0 - 0x40C0];
    char x4EF0[0x6420 - 0x4EF0];
    char x6420[0x792C - 0x6420];
    struct Src *x792C;
};

struct Obj {
    struct World *world;
    char pad4[0x3C];
    Vec x40;
    char pad50[0x10];
    Vec x60;
    char pad70[0x2C];
    float x9C;
    int xA0;
    int xA4;
    int xA8;
    int xAC;
    int xB0;
    int xB4;
    int xB8;
    int xBC;
    int xC0;
    int xC4;
    unsigned long flags;
};

extern void func_001A5218(void *p);
extern void func_00134608(void *p);
extern void func_001350E8(void *p);

#if defined(__mips__)
extern void func_001554B8(struct Obj *obj, float f);
#else

#endif



void func_00155148(struct Obj *obj)
{
    Vec *src;

    obj->xB0 = 0;
    obj->xC4 = 0;
    obj->x9C = 1.0f;
    obj->flags = (obj->flags & ~4L) | 8;
    obj->xB4 = 0;
    obj->xBC = 0;
    obj->xC0 = 0;
    obj->xA4 = 0;
    obj->xA8 = 0;
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x60));
    func_001A5218(obj->world->x4EF0);
    func_00134608(obj->world->x6420);
    func_001350E8(obj->world->x40C0);
    src = &obj->world->x792C->x10;
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(obj->x40) : "r"(src) : "$2");
    CallScalarBridge001554B8_6_func_001554B8(obj, 1.0f);
    func_00156568((RiderSpeedScale *)(obj), 1.0f);
}
