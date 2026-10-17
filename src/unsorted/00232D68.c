/* UNIT 0x00232D68 0x00232DA0 */
typedef struct Vec4 { float x, y, z, w; } Vec4;
typedef struct Obj { char pad0[0x60]; Vec4 x60, x70; char pad80[0x40]; Vec4 xC0, xD0; } Obj;

/* libvu0-style 128-bit vector copy (inline asm) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_00232D68(Obj *src, Obj *dst) {
    COPY_VEC(dst->x60, &src->x60);
    COPY_VEC(dst->x70, &src->x70);
    COPY_VEC(dst->xC0, &src->xC0);
    COPY_VEC(dst->xD0, &src->xD0);
}
