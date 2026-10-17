/* UNIT 0x0016F6E0 0x0016F710 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x1D0];
    struct Obj *x1F0;
} Obj;

extern void func_0016FC00(Obj *obj, int arg);

void func_0016F6E0(Obj *obj)
{
    Vec4 *src = &obj->x10;

    /* 128-bit vector copy (libvu0-style inline asm, same as func_0018FF10) */
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(obj->x1F0->x10) : "r"(src) : "$2");
    func_0016FC00(obj, 0);
}
