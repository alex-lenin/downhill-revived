/* UNIT 0x001AE788 0x001AE878 .rodata=0x28E930:0x28E940 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Matrix {
    Vec4 row[4];
} Matrix;

typedef struct Body {
    char pad0[0x40];
    Matrix mat;
} Body;

typedef struct Rider {
    char pad0[0x16D0];
    int x16D0;
    Body *body;
} Rider;

typedef struct Shared {
    char pad0[0xA10];
    Rider *rider;
} Shared;

typedef struct Obj {
    char pad0[0x20];
    Vec4 points[4];
    int x60;
} Obj;

extern Vec4 D_007684B0[];
extern int func_001B1C08(Rider *);
extern void func_001AE878(Obj *);

/* dst.xyz = m * v.xyz (VU0 macro mode) */
static inline void ApplyMatrix(Vec4 *dst, Matrix *m, Vec4 *v)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "lqc2 $vf13, 0x10(%2)\n"
        "lqc2 $vf14, 0x20(%2)\n"
        "vmulax.xyz ACC, $vf12, $vf11x\n"
        "vmadday.xyz ACC, $vf13, $vf11y\n"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*dst) : "r"(v), "r"(m) : "memory");
}

/* dst.xyz = a.xyz + b.xyz (VU0 macro mode) */
static inline void VecAdd(Vec4 *dst, Vec4 *a, Vec4 *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*dst) : "r"(a), "r"(b) : "memory");
}

void func_001AE788(Obj *obj, Shared *shared)
{
    Vec4 world;
    int base;
    int i;

    obj->x60 = shared->rider->x16D0;
    base = func_001B1C08(shared->rider);
    {
        int offsets[4] = { 0, 3, 6, 9 };

        for (i = 0; i < 4; i++) {
            int index = base + offsets[i] + 2;

            ApplyMatrix(&world, &shared->rider->body->mat, &obj->points[i]);
            VecAdd(&D_007684B0[index], &D_007684B0[index], &world);
        }
    }
    func_001AE878(obj);
}
