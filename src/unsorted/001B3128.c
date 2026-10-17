/* UNIT 0x001B3128 0x001B3170 */
#include "camera.h"
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef RiderCamera Obj;

void func_001B3128(Obj *obj) {
    int i;
    char *p = (char *)obj + 0x10;

    obj->xF4 = 1.0f;
    obj->x110 = 0;
    obj->x10C = 0;

    i = 0;
    do {
        COPY_VECTOR(*(Vec *)(p + 0x10), p);
        p = p + i - i;
        i++;
        p += 0x30;
    } while (i < 4);
}
