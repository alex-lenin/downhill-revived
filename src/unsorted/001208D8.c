/* UNIT 0x001208D8 0x00120AA8 */
#include "rider_geometry.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef float Vec4[4] __attribute__((aligned(16)));
typedef struct V4 { float v[4]; } V4 __attribute__((aligned(16)));

typedef struct VEntry {
    short delta;
    short pad2;
    char *(*fn)(void *);
} VEntry;

typedef struct Body {
    char pad0[0x10];
    float x10;
    float x14;
    float x18;
    char pad1C[0x34];
    V4 x50;
    Vec4 x60;
} Body;

typedef struct Obj {
    VEntry *vtbl;
    char pad4[0xC];
    Body *x10;
    char pad14[0xC];
    Vec4 x20;
    char pad30[0x130];
    float x160[4];
    Vec4 x170;
    Vec4 x180;
    char pad190[0x40];
    Vec4 x1D0;
    char pad1E0[0x82];
    short x262;
} Obj;


extern float func_00235EA8(float, float, float, float);
extern void func_00222F60(float *out, float *a, float *b);
extern void func_00223578(float *out, float *in, float t);
extern void func_00223210(float *out, float *a, float *b);
extern void func_002230B0(float *out, float *in);
extern void func_002232C8(char *p, float *q);
extern void func_0011E568(Obj *obj, int state);

static inline float Dot(float *a, float *b)
{
    float r;

    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
                      : "=f"(r) : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    return r;
}

#define COPY_VECTOR(dst, src)     __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* copies xyz, keeps dst's w */
static inline void CopyXYZ(float *dst, float *src)
{
    float w = dst[3];

    COPY_VECTOR(*(u128 *)dst, src);
    dst[3] = w;
}

void func_001208D8(Obj *obj) {
    Vec4 rot;
    V4 dir;
    VEntry *e;
    float d;
    float *up;
    float *normal;
    float *pos;
    Body *body;
    V4 *src;
    float x, y, z, len, inv;

    e = &obj->vtbl[6];
    d = func_00121348((GroundRider *)(obj), (void *)(e->fn((char *)obj + e->delta)), 0);
    obj->x10->x18 = func_00235EA8(obj->x10->x18, obj->x10->x18 - d, 0.04f, 0.001f);
    body = obj->x10;
    up = obj->x1D0;
    normal = body->x60;
    if (Dot(up, normal) < 0.99f) {
        func_00222F60(rot, normal, up);
        func_00223578(rot, rot, 0.05f);
        func_00223210(obj->x20, obj->x20, rot);
        func_002230B0(obj->x20, obj->x20);
        e = &obj->vtbl[4];
        func_002232C8(e->fn((char *)obj + e->delta) + 0x40, obj->x20);
        return;
    }
    __asm__ volatile ("sqc2 $vf0,%0" : "=m"(rot));
    src = &body->x50;
    COPY_VECTOR(dir, src);
    dir.v[2] = 0.0f;
    x = dir.v[0];
    y = dir.v[1];
    z = dir.v[2];
    __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    dir.v[0] = x * inv;
    dir.v[1] = y * inv;
    dir.v[2] = z;
    pos = obj->x160;
    CopyXYZ(pos + 4, rot);
    CopyXYZ(pos + 8, dir.v);
    pos[0] = obj->x10->x10;
    pos[1] = obj->x10->x14;
    obj->x262 = 0;
    func_0011E568(obj, 5);
}
