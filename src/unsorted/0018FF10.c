/* UNIT 0x0018FF10 0x0018FF30 */
typedef struct Vec4 { float x, y, z; int w; } Vec4;
typedef struct Obj { int x0; struct Obj *x4; char pad8[8]; Vec4 x10; } Obj;

void func_0018FF10(Obj *obj) {
    Obj *other = obj->x4;
    if (other) {
        /* libvu0-style 128-bit vector copy (inline asm) */
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(other->x10) : "r"(&obj->x10) : "$2");
    }
}
