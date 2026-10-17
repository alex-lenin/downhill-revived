/* UNIT 0x001DD460 0x001DD500 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    Vec x0;
    Vec x10;
    char pad20[0x30];
    float x50;
} Obj;

extern void func_001DD350(Obj *obj, Vec *out);

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline float Normalize(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

static inline void Scale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

void func_001DD460(Obj *obj, Vec *out, Vec *pos)
{
    Vec v;

    VecSub(&v, pos, &obj->x0);
    Normalize(&v, &v);
    Scale(&v, &v, obj->x50);
    VecSub(out, &v, &obj->x10);
    func_001DD350(obj, out);
}
