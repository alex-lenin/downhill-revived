/* UNIT 0x001C30A8 0x001C3178 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Matrix {
    Vec4 row[4];
} Matrix;

typedef struct Obj {
    char pad0[0x4];
    unsigned char x4;
    char pad5[0x10 - 0x5];
    Vec4 x10;
    char pad20[0x40 - 0x20];
    Matrix x40;
} Obj;

extern Matrix *func_0023CF58(int);
extern Vec4 *func_0023CF78(int);

/* 128-bit copies (libvu0-style inline asm) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
                      : "=m"((dst).row[0]), "=m"((dst).row[1]), "=m"((dst).row[2]), "=m"((dst).row[3]) \
                      : "r"(src) : "$8", "$9", "$10", "$11")

void func_001C30A8(void *unused, Obj *obj, Vec4 *local)
{
    Vec4 world;
    Matrix *mat;

    if (obj == 0) {
        return;
    }
    mat = &obj->x40;
    COPY_MATRIX(*mat, func_0023CF58(0));
    COPY_VEC(obj->x10, func_0023CF78(0));
    /* world = mat * local (xyz) */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\t"
                      "vmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(world) : "r"(local), "r"(mat) : "memory");
    obj->x4 = 1;
    obj->x10.x += world.x;
    obj->x10.y += world.y;
    obj->x10.z += world.z;
}
