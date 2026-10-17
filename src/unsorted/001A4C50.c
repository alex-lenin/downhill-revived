/* UNIT 0x001A4C50 0x001A4CE0 */
#include "object_registry.h"
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Track {
    int x0;
    char pad4[0xC];
    Vec4 x10;
} Track;

typedef struct Level {
    char pad0[0x16D4];
    Track *x16D4;
    char pad16D8[0xA88];
    unsigned char x2160;
} Level;

typedef struct Obj {
    char pad0[0x10];
    Vec4 x10;
    int x20;
    char pad24[0xD80];
    Level *xDA4;
    char padDA8[0x40];
    int xDE8;
    char padDEC[0x8];
    int xDF4;
    char padDF8[0x4];
    int xDFC;
    int xE00;
} Obj;

extern Vec4 D_0043F9E0;


extern void func_001A86C8(Obj *obj, int mode);
extern void func_001A6D00(Obj *obj);

/* 128-bit vector copy (inline asm in the original) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"((dst)->x) : "r"(src) : "$2", "memory")

void func_001A4C50(Obj *obj, Level *level)
{
    obj->xDA4 = level;
    obj->xDE8 = 0;
    obj->xDF4 = RegisterObjectRetail((void *)(obj), (unsigned int)(level->x16D4->x0));
    if (obj->xDA4->x2160) {
        COPY_VEC(&D_0043F9E0, &obj->xDA4->x16D4->x10);
        COPY_VEC(&obj->x10, &obj->xDA4->x16D4->x10);
        obj->x20 = 13;
        func_001A86C8(obj, 13);
        func_001A6D00(obj);
        obj->xDFC = 0;
        obj->xE00 = 0;
    }
}
