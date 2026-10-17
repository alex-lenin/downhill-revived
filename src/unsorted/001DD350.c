/* UNIT 0x001DD350 0x001DD460 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    Vec pos;
    Vec vel;
    Vec dir;
} Obj;

static inline float VecDot(Vec *a, Vec *b)
{
    float dot;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
             : "=f"(dot) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return dot;
}

static inline float VecNormalize(Vec *out, Vec *v)
{
    float length, inv;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\t"
                      "lui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(length)
                      : "f"(v->x), "f"(v->y), "f"(v->z), "f"(v->z)
                      : "$2");
    out->x = v->x * inv;
    out->y = v->y * inv;
    out->z = v->z * inv;
    return length;
}

static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void VecAddScaled(Vec *out, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline float Max(float a, float b)
{
    float result;
    __asm__ ("max.s %0, %1, %2" : "=&f"(result) : "f"(a), "f"(b));
    return result;
}

void func_001DD350(Obj *obj, Vec *v)
{
    Vec along;
    Vec unit;
    float length;
    float part;

    if (0.0f < VecDot(v, &obj->dir)) {
        length = VecNormalize(&unit, v);
        part = length * Max(0.2f, VecDot(&unit, &obj->dir));
        VecScale(&along, &obj->dir, part);
        part = length - part;
        VecAddScaled(v, &along, &unit, part);
    } else {
        part = 0.5f;
        VecScale(v, v, part);
    }
}
