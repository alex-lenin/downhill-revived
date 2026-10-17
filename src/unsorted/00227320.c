/* UNIT 0x00227320 0x00227518 */
#include "gs_packet.h"
typedef struct Obj {
    char pad0[0x272C]; float x272C;
    char pad2730[0x10]; float x2740, x2744;
    char pad2748[8]; float x2750, x2754;
    char pad2758[0x70]; float x27C8, x27CC, x27D0, x27D4;
    char pad27D8[4]; float x27DC;
    char pad27E0[0x44]; int x2824;
    char pad2828[8]; void *x2830, *x2834, *x2838;
    char pad283C[0x18]; int x2854;
} Obj;
typedef union Word { float f; int i; } Word;


extern int func_002249A8(int, int);

void func_00227320(Obj *obj) {
    Word *p, *base;
    float x0, y0, z0, w0, x1, y1, z1, w1;

    p = (Word *)(((unsigned int)obj + 12) & ~15U);
    base = p;
    x1 = obj->x2744; x0 = obj->x2740; y0 = obj->x2750; z0 = obj->x27C8;
    w0 = obj->x27D0; y1 = obj->x2754; z1 = obj->x27CC; w1 = obj->x27D4;
    obj->x2854 = 0x100;
    p = (Word *)func_002246A0((unsigned int *)(p), obj->x2824, 2);
    p[0].i = 1; p[2].i = 0x1A; p[4].i = 0x7C;
    p[1].i = 0; p[3].i = 0; p[5].i = 0; p[6].i = 0; p[7].i = 0;
    p[8].i = 0x6C040208;
    p = (Word *)func_00224660((GsPacket *)(p + 9), 0, 0x412412, 6, 0, 0, 0, 0, 0);
    p[8].f = 1.0f;
    p[0].f = 1.0f / (x0 - x1);
    p[1].f = 1.0f / (y0 - y1);
    p[2].f = 1.0f / (z0 - z1);
    p[3].f = 1.0f / (w0 - w1);
    p[5].f = 1.0f - p[1].f * y0;
    p[6].f = 1.0f - p[2].f * z0;
    p[7].f = 1.0f - p[3].f * w0;
    p[4].f = 1.0f - p[0].f * x0;
    p[9].f = 0.234375f;
    p[10].f = obj->x27DC;
    p[11].f = obj->x272C;
    p[12].i = 0;
    p[13].i = 0;
    p += 15;
    obj->x2830 = p;
    p += 0x400;
    base[1].i = (int)p;
    p[1].i = 0; p[2].i = 0;
    p += 4;
    obj->x2838 = p;
    p = (Word *)((char *)p + (func_002249A8(obj->x2824, 0) * 4 + 0x400));
    obj->x2834 = p;
}
