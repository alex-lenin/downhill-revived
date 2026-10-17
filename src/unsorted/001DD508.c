/* UNIT 0x001DD508 0x001DD668 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    Vec x0;
    Vec x10;
    Vec x20;
    char pad30[0x14];
    float x44;
} Obj;

typedef struct Target {
    Vec pos;
    float x10;
    float x14;
    float dist;
} Target;

extern void func_001DD460(Obj *obj, Vec *out, Vec *pos);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

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
    __asm__ volatile ("mula.s %3, %3
	madda.s %4, %4
	madd.s %1, %5, %5
	sqrt.s %1, %1
	"
                      "lui %2, 0x3f80
	mtc1 %2, %0
	nop
	div.s %0, %0, %1"
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

static inline float Dot(Vec *a, Vec *b)
{
    float d;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(d) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return d;
}

void func_001DD508(Obj *obj, Vec *out, Target *target)
{
    Vec dir;
    Vec v;
    float dist, t;

    VecSub(&dir, &target->pos, &obj->x0);
    dist = Normalize(&dir, &dir);
    target->dist = dist;
    if (target->x10 < dist) {
        COPY_VECTOR(v, &target->pos);
        func_001DD460(obj, out, &v);
        return;
    }
    t = dist / target->x10;
    Scale(&v, &dir, target->x14 + t * t * (obj->x44 - target->x14));
    if (Dot(&v, &obj->x20) < 0.5f && dist < 2.0f) {
        Scale(out, &obj->x10, -1.0f);
    } else {
        VecSub(out, &v, &obj->x10);
    }
}
