/* UNIT 0x0019D120 0x0019D588 .sdata=0x29D1B8:0x29D1C0 */
#include "spatial.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Hit {
    unsigned short x0, x2, x4;
    char pad6[2];
    float x8;
    float xC;
    Vec x10;
    Vec x20;
} Hit __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x500];
    char *x500;
    char pad504[0xC];
    Vec x510;
} Obj;

typedef struct Rider {
    char pad0[0x10];
    Vec x10;
} Rider;

float D_0029D1B8 = 8.0f;
float D_0029D1BC = 0.1f;

extern void func_00254204(void *dst, int value, int size);

extern void func_0019CDA0(Obj *obj);
extern Rider *func_0023D040(int index);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Scale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void Add(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Sub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void AddScaled(Vec *out, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline float Dot(Vec *a, Vec *b)
{
    float out;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(out)
                      : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return out;
}

static inline float Abs(float x)
{
    return __builtin_fabsf(x);
}

static inline float Normalize(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
    return len;
}

void func_0019D120(Obj *obj)
{
    Hit hits[10];
    Vec push;
    Vec scaled;
    Vec target;
    Vec delta;
    Hit *hit;
    Vec *dir;
    Rider *rider;
    float len;
    float d0;
    float d1;
    float limit;
    float sx, asx, px, apx;
    float sy, asy, py, apy;
    float sz, asz, pz, apz;
    int i;
    int m;
    int j;
    int n;
    int k;
    int count;
    int best;

    COPY_VECTOR(obj->x510, obj->x500 + 0x10);

    for (i = 0; i < 10; i++) {
        func_00254204(hits, 0, sizeof(hits));
        n = func_00214A18((Vector4 *)(&obj->x510), (Vector4 *)(&obj->x510), (SpatialHit *)(hits), 10, 1.2f, 0.0f, 1.0f);
        if (n == 0) {
            break;
        }
        push.x = 0.0f;
        push.y = 0.0f;
        push.z = 0.0f;
        for (k = 0; k < n; k++) {
            if (hits[k].x0 != 0 && !(0.0f <= hits[k].x10.w)) {
                Scale(&scaled, &hits[k].x20, hits[k].x8);
                /* Keep the larger push per axis. Zero-byte labels (cf. src/unsorted/0010E9B0.c):
                   the first keeps each scaled load + abs.s ahead of the push load (retail
                   order), the second keeps the push value live past its abs.s so the result
                   gets its own register (retail allocation). */
                sx = scaled.x;
                asx = Abs(sx);
                __asm__ volatile ("L19d120_x0:");
                px = push.x;
                apx = Abs(px);
                __asm__ volatile ("L19d120_x1:" : : "f"(px));
                if (apx < asx) push.x = sx;
                sy = scaled.y;
                asy = Abs(sy);
                __asm__ volatile ("L19d120_y0:");
                py = push.y;
                apy = Abs(py);
                __asm__ volatile ("L19d120_y1:" : : "f"(py));
                if (apy < asy) push.y = sy;
                sz = scaled.z;
                asz = Abs(sz);
                __asm__ volatile ("L19d120_z0:");
                pz = push.z;
                apz = Abs(pz);
                __asm__ volatile ("L19d120_z1:" : : "f"(pz));
                if (apz < asz) push.z = sz;
            }
        }
        Add(&obj->x510, &obj->x510, &push);
    }

    func_0019CDA0(obj);
    rider = func_0023D040(0);

    for (m = 0; m < 10; m++) {
        func_00254204(hits, 0, sizeof(hits));
        count = func_00214A18((Vector4 *)(&obj->x510), (Vector4 *)(&rider->x10), (SpatialHit *)(hits), 10, 1.2f, 0.0f, 1.0f);
        if (count == 0) {
            break;
        }
        best = -1;
        for (j = 0; j < count; j++) {
            if (hits[j].x0 != 0 && !(hits[j].x10.w < 0.0f)) {
                if (best < 0 || hits[j].x10.w < hits[best].x10.w) {
                    best = j;
                }
            }
        }
        if (best < 0) {
            continue;
        }
        hit = &hits[best];
        if (hit->x0 == 0 || !(0.0f <= hit->x10.w)) {
            continue;
        }
        Sub(&delta, &rider->x10, &obj->x510);
        dir = &delta;
        Scale(dir, dir, hit->x10.w);
        len = Normalize(&delta);
        limit = D_0029D1B8;
        if (len < limit) {
            d0 = Dot(&hit->x20, &hit->x10);
            d1 = Dot(&hit->x20, &rider->x10);
            Scale(dir, &hit->x20, 1.2f - (d1 - d0));
            Add(&target, &rider->x10, dir);
            Sub(dir, &target, &obj->x510);
            len = Normalize(&delta);
            if (!(D_0029D1BC <= len)) {
                Sub(dir, &rider->x10, &obj->x510);
                AddScaled(&rider->x10, &rider->x10, dir, -2.0f);
                continue;
            }
        }
        AddScaled(&rider->x10, &obj->x510, dir, limit);
    }
}
