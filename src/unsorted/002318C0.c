/* UNIT 0x002318C0 0x002318F8 */
typedef struct Vec4 { float x, y, z, w; } Vec4;
typedef struct Obj {
    int x0;
    char pad4[0xC];
    Vec4 x10;
    char pad20[0x14];
    int x34, x38, x3C, x40;
    char pad44[0xC];
    Vec4 x50;
} Obj;

/* libvu0-style 128-bit vector copy (inline asm) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

Obj *func_002318C0(Obj *obj) {
    Vec4 *v;
    obj->x0 = 0;
    obj->x34 = 0;
    obj->x38 = 0;
    obj->x3C = 0;
    obj->x40 = 0;
    v = &obj->x10;
    v->x = 0;
    v->y = 0;
    v->z = 0;
    COPY_VEC(obj->x50, v);
    return obj;
}
