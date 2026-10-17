/* UNIT 0x00155090 0x00155148 */
#include "rider_geometry.h"
typedef struct Obj {
    int x0;
    char pad4[0x60 - 0x4];
    float x60[4] __attribute__((aligned(16)));
    char pad70[0xA0 - 0x70];
    float xA0;
    int xA4;
    int xA8;
    char padAC[0xB8 - 0xAC];
    int xB8;
    char padBC[0xC0 - 0xBC];
    int xC0;
    int xC4;
    unsigned long b0 : 1;
    unsigned long b1 : 1;
    unsigned long b2 : 1;
    unsigned long b3 : 1;
    unsigned long b4 : 1;
    unsigned long b5 : 1;
    unsigned long b6 : 1;
    unsigned long b7 : 1;
    unsigned long rest : 56;
} Obj;

extern void *func_00254204(void *dst, int value, int size);
extern void func_001564A8(Obj *obj);


void func_00155090(Obj *obj, int arg)
{
    float scale = 1.0f;

    func_00254204(obj, 0, 0xD0);
    obj->b0 = 1;
    obj->b1 = 1;
    obj->b5 = 0;
    obj->b6 = 0;
    obj->b4 = 0;
    obj->b7 = 0;
    obj->x0 = arg;
    obj->xC4 = 0;
    obj->xB8 = 0;
    obj->xC0 = 0;
    obj->xA0 = scale;
    obj->xA4 = 0;
    obj->xA8 = 0;
    __asm__ volatile ("sqc2 $vf0, %0" : "=m" (obj->x60));
    func_001564A8(obj);
    func_00156568((RiderSpeedScale *)(obj), scale);
}
