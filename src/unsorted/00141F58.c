/* UNIT 0x00141F58 0x00142238 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Point { Vec pos; short x10, x12; unsigned short x14; short x16; } Point;
typedef struct Body { char pad0[4]; unsigned char x4; char pad5[0xB]; Vec x10; } Body;
typedef struct Obj {
    Vec x0;
    char pad10[0x40];
    int x50, x54, x58;
    char pad5C[0x2C];
    Point *x88, *x8C, *x90;
    char pad94[0x0C];
    Vec xA0, xB0, xC0, xD0, xE0;
    Body *xF0;
    char padF4[0x6C];
    float x160;
} Obj;

extern char D_003C2230[];
extern int func_00222B08(int, int);
extern Point *func_00163DA0(void *, Point *, int);
extern void func_00141C00(Vec *, Vec *, Point *, Point *, float);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VecAddScaled(Vec *out, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tlqc2 $vf12, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf13\n\t"
                      "vadda.xyz ACC, $vf11, $vf0\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, 0x0(%3)"
                      : : "r"(a), "r"(b), "f"(s), "r"(out) : "$2", "memory");
}

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecScale(Vec *out, Vec *a, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "f"(s) : "$2");
}

static inline float Dot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
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

void func_00141F58(Obj *obj)
{
    Vec delta, tmp;
    Body *body;
    Point *next;
    Point *a, *b;
    float proj, bound, dist, frac;

    body = obj->xF0;
    VecAddScaled(&obj->xE0, &body->x10, &obj->x0, 20.0f);
    VecSub(&delta, &obj->xE0, &obj->x88->pos);
    proj = Dot(&obj->xA0, &delta);
    bound = obj->xA0.w;
    if (bound < proj) {
        if (obj->x50 != 5) {
            obj->x88 = obj->x8C;
            obj->xA0 = obj->xB0;
            next = obj->x90;
            obj->x8C = next;
            if (next->x16 == 0
                || (((next->x14 >> 1) & 1) && (func_00222B08(0x80000001, 0x7FFFFFFF) & 0xFF) < 0x7F)) {
                obj->x50 = 5;
                obj->x58 = -1;
            } else {
                obj->x90 = func_00163DA0(D_003C2230, obj->x90, 0);
                func_00141C00(&obj->xA0, &obj->xB0, obj->x8C, obj->x90, obj->x8C->x10);
            }
            VecSub(&delta, &obj->xE0, &obj->x88->pos);
            proj = Dot(&obj->xA0, &delta);
        } else {
            VecSub(&tmp, &body->x10, &obj->x88->pos);
            dist = Dot(&obj->xA0, &tmp);
            obj->x160 = dist;
            if (bound <= dist) {
                obj->x50 = 0;
                body->x4 = 0;
            }
        }
    }
    if (proj < 0.0f) {
        COPY_VECTOR(obj->xC0, &delta);
        obj->xE0.w = obj->x88->x10;
    } else {
        VecScale(&tmp, &obj->xA0, proj);
        VecSub(&obj->xC0, &delta, &tmp);
        frac = proj / obj->xA0.w;
        a = obj->x88;
        b = obj->x8C;
        obj->xE0.w = a->x10 + frac * (b->x10 - a->x10);
    }
    obj->xC0.w = Normalize(&obj->xC0, &obj->xC0);
}
