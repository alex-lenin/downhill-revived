/* UNIT 0x0012AA58 0x0012AC28 */
#include "rider_geometry.h"
typedef unsigned int u128 __attribute__((mode(TI)));

struct Vec4 {
    union {
        u128 q;
        float f[4];
    };
};

struct Body {
    char pad0[0x10];
    float x10;
    float x14;
    float x18;
    char pad1C[0x50 - 0x1C];
    Vec4 x50;
    float x60[4];
};

struct Model {
    char pad0[0x40];
    char x40[0x40];
};

class Base {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Model *v3();
    virtual void v4();
    virtual int v5();
};

struct Obj : Base {
    Body *x4;
    char pad8[0x10 - 0x8];
    float x10[4];
    char pad20[0xD0 - 0x20];
    float xD0[4];
    Vec4 xE0;
    Vec4 xF0;
    char pad100[0x130 - 0x100];
    float x130[4];
    char pad140[0x188 - 0x140];
    int x188;
};

extern "C" {

float func_00235EA8(float from, float to, float a, float b);
void func_00222F60(float *out, float *a, float *b);
void func_00223578(float *out, float *in, float scale);
void func_00223210(float *out, float *a, float *b);
void func_002230B0(float *out, float *in);
void func_002232C8(void *out, float *in);
void func_00129860(Obj *obj, int state);
}

static inline float Dot(const float *a, const float *b)
{
    float r;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r)
             : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    return r;
}

static inline void Copy(Vec4 *dst, const Vec4 *src)
{
    __asm__ volatile ("lq $2, 0(%1)
	sq $2, %0" : "=m"(dst->q) : "r"(src) : "$2");
}

/* copies x, y, z, keeping dst's w */
static inline void SetXYZ(Vec4 *dst, const Vec4 *src)
{
    float w = dst->f[3];
    Copy(dst, src);
    dst->f[3] = w;
}

extern "C" void func_0012AA58(Obj *obj)
{
    Vec4 tmp;
    Vec4 dir;
    float r;
    float scale;
    float lenSq;
    Vec4 *src;
    float *pos;
    Body *body;
    float *up;
    float *fwd;
    float xx, yy, zz;

    r = func_0012AD80((GroundActor *)(obj), (void *)(obj->v5()), 0);
    obj->x4->x18 = func_00235EA8(obj->x4->x18, obj->x4->x18 - r, 0.04f, 0.001f);
    fwd = obj->x130;
    body = obj->x4;
    up = body->x60;
    if (Dot(fwd, up) < 0.99f) {
        func_00222F60(tmp.f, up, fwd);
        func_00223578(tmp.f, tmp.f, 0.05f);
        func_00223210(obj->x10, obj->x10, tmp.f);
        func_002230B0(obj->x10, obj->x10);
        func_002232C8(obj->v3()->x40, obj->x10);
    } else {
        __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(&tmp) : "memory");
        Copy(&dir, &body->x50);
        dir.f[2] = 0.0f;
        xx = dir.f[0];
        yy = dir.f[1];
        zz = dir.f[2];
        __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3" : "=f"(lenSq)
                          : "f"(xx), "f"(yy), "f"(zz));
        __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(scale) : "f"(1.0f), "f"(lenSq));
        dir.f[0] = xx * scale;
        dir.f[1] = yy * scale;
        dir.f[2] = zz;
        pos = obj->xD0;
        SetXYZ(&obj->xE0, &tmp);
        SetXYZ(&obj->xF0, &dir);
        pos[0] = obj->x4->x10;
        pos[1] = obj->x4->x14;
        obj->x188 = 0;
        func_00129860(obj, 5);
    }
}
