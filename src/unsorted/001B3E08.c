/* UNIT 0x001B3E08 0x001B3E98 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x60];
    u128 x60;
    char pad70[0xC0 - 0x70];
    int xC0;
    char padC4[0xD0 - 0xC4];
    u128 xD0;
} Obj;

extern u128 D_007685A0[];
extern int func_001B1C08(int);

static inline void ApplyMatrix(void *out, void *m, void *v)
{
    __asm__ volatile ("lqc2 $vf11, 0(%2)\n\tlqc2 $vf12, 0(%1)\n\tlqc2 $vf13, 0x10(%1)\n\tlqc2 $vf14, 0x20(%1)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, 0(%0)" : : "r"(out), "r"(m), "r"(v) : "memory");
}

static inline void AddVector(void *out, void *a, void *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%0)" : : "r"(out), "r"(a), "r"(b) : "memory");
}

static inline void CopyVector(u128 *out, u128 *v)
{
    __asm__ volatile ("lq $2, 0(%1)
	sq $2, %0" : "=m"(*out) : "r"(v) : "$2");
}

void func_001B3E08(Obj *obj, void *rot, void *offset)
{
    u128 tmp;
    int idx;

    idx = func_001B1C08(obj->xC0);
    ApplyMatrix(&tmp, rot, &obj->xD0);
    AddVector(&D_007685A0[idx], &tmp, offset);
    CopyVector(&obj->x60, &D_007685A0[idx]);
}
