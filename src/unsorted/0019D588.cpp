/* UNIT 0x0019D588 0x0019DE20 .sdata=0x29D1C0:0x29D1C8 */
#include "spatial.h"
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

/* pos.w holds the hit fraction */
typedef struct Hit {
    unsigned short found;
    char pad2[0xE];
    Vec pos;
    Vec normal;
} Hit;

typedef struct QueryFlags {
    unsigned int x0;
    union {
        unsigned int word;
        unsigned short x4;
        struct { unsigned int low:16, a:1, b:1, c:1, other:13; } bits;
    } x4;
    int pad8[2];
} QueryFlags;

typedef struct Body {
    char pad0[0x10];
    Vec x10;
} Body;

typedef struct Obj {
    char pad0[0x45C];
    unsigned int x45C;
    char pad460[0x500 - 0x460];
    Body *x500;
    char pad504[0xC];
    Vec x510;
} Obj;

typedef struct Camera {
    char pad0[0x10];
    Vec pos;
    char pad20[0x20];
    Vec x40;
    Vec x50;
    Vec x60;
} Camera;

typedef struct Pad {
    char pad0[0x64];
} Pad;

float D_0029D1C0 = 20.0f;
float D_0029D1C4 = 8.0f;

extern Pad D_003F52D0[];

static inline Pad *GetPad(int i)
{
    return &D_003F52D0[i];
}
extern Camera *func_0023D040(int);
extern unsigned char func_00190990(void *pad, int col, int row);
extern unsigned int func_00190D88(void *pad, int i, unsigned int mask);
extern void func_00222E80(float *quat, Vec *axis, float angle);
extern void func_002232C8(float *mat, float *quat);

extern QueryFlags *func_00210940(QueryFlags *);

extern void func_00254204(void *, int, int);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VAdd(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VSub(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VScale(Vec *dst, Vec *src, float f)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(src), "f"(f) : "$2");
}

static inline void VCross(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline float Normalize(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, length, inv;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %0,%4,%5\n\tsqrt.s %0,%0\n\t"
                      "lui $2,0x3f80\n\tmtc1 $2,%1\n\tnop\n\tdiv.s %1,%1,%0"
                      : "=&f"(length), "=&f"(inv) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
    return length;
}

/* same as Normalize, z passed once */
static inline float NormalizeLen(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, length, inv;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %0,%4,%4\n\tsqrt.s %0,%0\n\t"
                      "lui $2,0x3f80\n\tmtc1 $2,%1\n\tnop\n\tdiv.s %1,%1,%0"
                      : "=&f"(length), "=&f"(inv) : "f"(x), "f"(y), "f"(z) : "$2");
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
    return length;
}

/* v->z is known to be 0: only x and y are scaled */
static inline void NormalizeXY(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, length, inv;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %0,%4,%5\n\tsqrt.s %0,%0\n\t"
                      "lui $2,0x3f80\n\tmtc1 $2,%1\n\tnop\n\tdiv.s %1,%1,%0"
                      : "=&f"(length), "=&f"(inv) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    v->x = x * inv;
    v->y = y * inv;
}

static inline float Dot(Vec *a, Vec *b)
{
    float ax = a->x, bx = b->x, ay = a->y, by = b->y, az = a->z, bz = b->z, result;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
                      : "=f"(result) : "f"(ax), "f"(bx), "f"(ay), "f"(by), "f"(az), "f"(bz));
    return result;
}

static inline float Sqrt(float x)
{
    float r;
    __asm__ volatile ("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float maxf(float a, float b)
{
    float r;
    __asm__ ("max.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float minf(float a, float b)
{
    float r;
    __asm__ ("min.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float minv(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float maxv(float a, float b)
{
    return a >? b;
}

static inline float Clamp(float x, float lo, float hi)
{
    return (x >? lo) <? hi;
}

static inline float Range(float lo, float x, float hi)
{
    return (lo >? x) <? hi;
}

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

static inline float Decay(float value, float amount)
{
    return (value - Abs(amount)) >? 0.0f;
}

static inline void Init(QueryFlags *f)
{
    f->x0 = -1;
    f->x4.x4 = 0xffff;
    f->x4.bits.a = 0;
    f->x4.bits.b = 0;
    f->x4.bits.c = 0;
}

void func_0019D588(Obj *obj)
{
    Vec dir;
    float quat[4];
    float mat[16];
    Vec tmp;
    Hit hits[10];
    QueryFlags query;
    Vec d;
    Camera *cam;
    Vec *up;
    signed char rx, ry, lx, ly;
    float length, dist, angle, z, s, one;
    float slack, range, hitLength, diff, clamped, planeDist;
    int count, best, i, j, done;
    QueryFlags *old;

    cam = func_0023D040(0);
    VSub(&dir, &obj->x510, &cam->pos);
    length = NormalizeLen(&dir);
    dist = length;
    angle = 0.0f;
    rx = func_00190990(GetPad(1), 2, 0);
    ry = func_00190990(GetPad(1), 3, 0);
    lx = func_00190990(GetPad(1), 0, 0);
    ly = func_00190990(GetPad(1), 1, 0);
    rx -= 0x80;
    ry -= 0x80;
    lx -= 0x80;
    ly -= 0x80;
    if (func_00190D88(GetPad(1), 0, obj->x45C)) {
        ry = -0x50;
    } else if (func_00190D88(GetPad(1), 1, obj->x45C)) {
        ry = 0x50;
    }
    if (func_00190D88(GetPad(1), 2, obj->x45C)) {
        rx = -0x50;
    } else if (func_00190D88(GetPad(1), 3, obj->x45C)) {
        rx = 0x50;
    }
    if (rx < -0x40) {
        angle = 0.034906585f;
    } else if (rx > 0x40) {
        angle = -0.034906585f;
    }
    if (angle != 0.0f) {
        func_00222E80(quat, &cam->x60, angle);
        func_002232C8(mat, quat);
        func_00221888((const Matrix4 *)(mat), (const Vector4 *)(&dir), (Vector4 *)(&dir));
    }
    angle = 0.0f;
    if (ry < -0x40) {
        angle = 0.034906585f;
    }
    if (ry > 0x40) {
        angle = -0.034906585f;
    }
    if (ly < -0x40) {
        dist = length - 0.5f;
    } else if (ly > 0x40) {
        dist = length + 0.5f;
    }
    if (angle != 0.0f) {
        func_00222E80(quat, &cam->x40, angle);
        func_002232C8(mat, quat);
        func_00221888((const Matrix4 *)(mat), (const Vector4 *)(&dir), (Vector4 *)(&dir));
    }
    if (Abs(dir.z) > 0.9f) {
        one = 1.0f;
        z = minv(maxf(dir.z, -0.9f), 0.9f);
        s = Sqrt(one - z * z);
        if (dir.z == one) {
            tmp.x = 0.0f;
            tmp.y = s;
            tmp.z = 0.0f;
        } else {
            COPY_VECTOR(tmp, &dir);
            tmp.z = 0.0f;
            NormalizeXY(&tmp);
            VScale(&tmp, &tmp, s);
        }
        tmp.z = z;
        COPY_VECTOR(dir, &tmp);
    }
    COPY_VECTOR(tmp, &cam->pos);
    dist = Clamp(dist, D_0029D1C4, D_0029D1C0);
    VScale(&dir, &dir, dist);
    VSub(&cam->pos, &obj->x510, &dir);

    Init(&query);
    query.x0 = 0;
    done = 0;
    slack = 0.5f;
    range = 2.0f;
    old = func_00210940(&query);
    for (i = 0; !done && i < 4; i++) {
        func_00254204(hits, 0, sizeof(hits));
        count = func_00214A18((Vector4 *)(&obj->x510), (Vector4 *)(&cam->pos), (SpatialHit *)(hits), 10, 1.2f, 0.0f, 1.0f);
        if (count == 0) {
            done = 1;
        } else {
            best = -1;
            for (j = 0; j < count; j++) {
                if (hits[j].found && !(hits[j].pos.w < 0.0f)
                    && (best < 0 || hits[j].pos.w < hits[best].pos.w)) {
                    best = j;
                }
            }
            if (best >= 0 && hits[best].found && hits[best].pos.w >= 0.0f) {
                VSub(&d, &cam->pos, &obj->x510);
                VScale(&d, &d, hits[best].pos.w);
                hitLength = Normalize(&d);
                diff = hitLength - dist;
                if (Abs(diff) < range) {
                    range -= diff;
                    planeDist = Dot(&hits[best].normal, &hits[best].pos);
                    VScale(&d, &hits[best].normal, 1.2f - (Dot(&hits[best].normal, &cam->pos) - planeDist));
                    VAdd(&cam->pos, &cam->pos, &d);
                } else {
                    float low, high, delta;
                    VSub(&tmp, &tmp, &obj->x510);
                    Normalize(&tmp);
                    low = dist - slack;
                    high = dist + slack;
                    clamped = Range(low, hitLength, high);
                    delta = clamped - dist;
                    slack = Decay(slack, delta);
                    range = Decay(range, delta);
                    VScale(&tmp, &tmp, clamped);
                    VAdd(&cam->pos, &obj->x510, &tmp);
                }
            }
        }
    }
    func_00210940(old);

    VSub(&dir, &obj->x500->x10, &cam->pos);
    Normalize(&dir);
    COPY_VECTOR(cam->x50, &dir);
    up = &cam->x60;
    up->x = 0.0f;
    up->z = 1.0f;
    up->y = 0.0f;
    VCross(&cam->x40, &cam->x50, up);
    NormalizeLen(&cam->x40);
    VCross(up, &cam->x40, &cam->x50);
}
