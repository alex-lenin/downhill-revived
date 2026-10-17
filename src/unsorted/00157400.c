/* UNIT 0x00157400 0x00157498 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Rider {
    char pad0[0x792C];
    Vec *x792C;
} Rider;

typedef struct Obj {
    Rider *x0;
    char pad4[0x2C];
    Vec x30;
    Vec x40;
    char x50[0x5C];
    int xAC;
    char padB0[0x18];
    unsigned long xC8;
} Obj;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

extern int func_00157498(Obj *obj, Vec *body, void *arg);

void func_00157400(Obj *obj, int a)
{
    Vec *dst;
    Vec *src;

    if (obj->xAC == 3 || obj->xAC == 4 || a != 0) {
        if (obj->xC8 & 0x10) {
            return;
        }
        dst = &obj->x30;
        src = &obj->x40;
    } else {
        if (obj->xC8 & 0x10) {
            return;
        }
        dst = &obj->x30;
        src = obj->x0->x792C;
        src += 1;
    }

    COPY_VECTOR(obj->x30, src);
    if (func_00157498(obj, dst, &obj->x50) != 0) {
        obj->xC8 |= 0x10;
    }
}
