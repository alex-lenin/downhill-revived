/* UNIT 0x00176930 0x00176A80 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Matrix {
    Vec4 row[4];
} Matrix;

typedef struct Game {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
} Game;

typedef struct Owner {
    char pad0[0x1280];
    int x1280;
} Owner;

typedef struct Obj {
    char pad0[0x4];
    unsigned char x4;
    char pad5;
    unsigned char x6;
    char pad7[0x10 - 0x7];
    Vec4 x10;
    char pad20[0x40 - 0x20];
    Matrix x40;
} Obj;

extern Game D_004B25A0;

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

void func_00176930(Owner *owner, Obj *obj, float *offset, float sx, float sy, float sz)
{
    Vec4 local;
    Vec4 world;
    Matrix *mat;

    if (obj == 0) {
        return;
    }
    if (D_004B25A0.x288 == 2) {
        local.x = offset[0] * sx;
        local.y = offset[1] * sy;
        local.z = offset[2] * sz;
    } else {
        local.x = offset[0];
        local.y = offset[1];
        local.z = offset[2];
    }
    mat = &obj->x40;
    COPY_MATRIX(*mat, func_0023CF58(owner->x1280));
    COPY_VEC(obj->x10, func_0023CF78(owner->x1280));
    /* world = mat * local (xyz) */
    __asm__ volatile ("lqc2 $vf11, %1\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\t"
                      "vmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(world) : "m"(local), "r"(mat) : "memory");
    obj->x10.x += world.x;
    obj->x10.y += world.y;
    obj->x10.z += world.z;
    obj->x6 = 1 << owner->x1280;
    if (D_004B25A0.x10C != 10) {
        obj->x4 = 1;
    } else {
        obj->x4 = 0;
    }
}
