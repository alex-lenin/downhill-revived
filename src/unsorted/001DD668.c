/* UNIT 0x001DD668 0x001DD7E8 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    Vec pos;
    char pad10[0x48];
    float x58;
} Obj;

typedef struct Target {
    Vec pos;
    float x10;
    float x14;
    float dist;
} Target;

typedef struct Waypoint {
    Vec pos;
    Vec alt;
    char pad20[0x10];
    float x30;
    float x34;
    float dist;
    unsigned char useAlt;
} Waypoint;

extern void func_001DD508(Obj *obj, Vec *out, Target *target);

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

void func_001DD668(Obj *obj, Vec *out, Waypoint *wp)
{
    Vec dir;
    Target target;
    float dist;
    float speed, base;
    int useAlt;

    useAlt = 0;
    if (wp->useAlt) {
        VecSub(&dir, &wp->alt, &obj->pos);
        dist = VecNormalize(&dir, &dir);
        if (dist < 2.0f || dist < wp->dist) {
            wp->useAlt = 0;
        } else {
            wp->dist = dist;
            useAlt = 1;
        }
    }
    VecSub(&dir, &wp->pos, &obj->pos);
    if (useAlt) {
        base = wp->x34;
        speed = obj->x58 - base;
        speed *= 0.67f;
        COPY_VECTOR(target.pos, &wp->alt);
        target.x10 = wp->x30 * 0.5f;
        target.x14 = speed;
    } else {
        COPY_VECTOR(target.pos, &wp->pos);
        target.x14 = wp->x34;
        target.x10 = wp->x30;
    }
    target.dist = wp->dist;
    func_001DD508(obj, out, &target);
    wp->dist = VecNormalize(&dir, &dir);
}
