/* UNIT 0x0012B528 0x0012B540 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct {
    char pad0[0xE0];
    Vec4 xE0;
} Obj;

/* quadword copy, as the game's vector-copy macro (sceVu0CopyVector style) */
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_0012B528(Obj *obj, int unused, Vec4 *dst) {
    COPY_VECTOR(*dst, &obj->xE0);
}
