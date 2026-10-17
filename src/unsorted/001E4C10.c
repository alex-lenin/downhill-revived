/* UNIT 0x001E4C10 0x001E4D30 */
typedef struct Obj {
    char pad0[0x10];
    float pos[4];
    char pad20[0x30];
    float x50[4];
    float x60[4];
} Obj;

typedef struct Vec {
    float x;
    float y;
    float z;
    int w;
} Vec;

extern Obj *D_004C5BD0[];

/* out = a + b * s */
static inline void ScaleAdd(float *out, float *a, float *b, float s)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(a), "r"(b), "f"(s) : "$2", "memory");
}

static inline void SubVector(float *out, float *a, float *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(a), "r"(b) : "memory");
}

static inline float Length(float *v)
{
    float len;

    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(len) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
    return len;
}

int func_001E4C10(Vec *out, Obj *obj, int index)
{
    float diff[4] __attribute__((aligned(16)));
    Obj *other;
    float *center;
    float dist;

    ScaleAdd(&out->x, obj->pos, obj->x60, -2.0f);
    ScaleAdd(&out->x, &out->x, obj->x50, 5.0f);
    out->w = 0;
    other = D_004C5BD0[index];
    if (other != 0) {
        center = other->pos;
        SubVector(diff, &out->x, center);
        dist = Length(diff);
        if (dist < 5.5f) {
            return 0;
        }
        if (dist > 9.0f) {
            return 1;
        }
        ScaleAdd(&out->x, center, diff, 6.0f / dist);
    }
    return 1;
}
