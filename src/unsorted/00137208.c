/* UNIT 0x00137208 0x00137300 */
typedef struct Vec4 {
    float v[4];
} Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec4 x10;
} Body;

typedef struct Obj {
    char pad0[0x38];
    Body *x38;
    char pad3C[0x158 - 0x3C];
    char *x158;
    char pad15C[0x270 - 0x15C];
    Vec4 x270;
} Obj;

extern float func_001DC0E8(void *p);
extern void func_001377A8(Obj *obj, Vec4 *dir);
extern void func_00137300(Obj *obj);

static inline void VecCross(Vec4 *dst, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecAdd(Vec4 *dst, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecScale(Vec4 *dst, Vec4 *a, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "f"(s) : "$2");
}

void func_00137208(Obj *obj)
{
    Vec4 cross;
    Vec4 dir;
    Vec4 *up;
    float t;
    float len;
    float inv;

    up = &obj->x38->x10;
    t = func_001DC0E8(obj->x158 + 0x621C);
    VecCross(&cross, up, &obj->x270);
    VecScale(&cross, &cross, t);
    VecScale(&dir, &obj->x270, 1.0f - t);
    VecAdd(&dir, &dir, &cross);
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(dir.v[0]), "f"(dir.v[1]), "f"(dir.v[2]), "f"(dir.v[2]) : "$2");
    dir.v[0] *= inv;
    dir.v[1] *= inv;
    dir.v[2] *= inv;
    func_001377A8(obj, &dir);
    func_00137300(obj);
}
