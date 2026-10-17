/* UNIT 0x001737F0 0x00173A18 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec pos;
    char pad20[0x30];
    float x50, x54, x58;
} Body;

typedef struct Obj {
    char pad0[0x4];
    int x4;
    char pad8[0x4];
    Body *xC;
    char pad10[0x40];
    Vec x50;
    char pad60[0x20];
    Vec x80;
    char pad90[0x28];
    float xB8;
    float xBC;
} Obj;

extern float func_00235EA8(float, float, float, float);
#if defined(__mips__)
extern float func_00211980(Vec *, Vec *, int, float);
#endif
extern void func_00211280(Body *);

extern float fabsf(float);
static inline void Scale(Vec *out, Vec *v, float scale) { __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v), "f"(scale) : "$2"); }
static inline void Add(Vec *out, Vec *a, Vec *b) { __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b)); }
static inline float Dot(Vec *a, Vec *b) { float r; __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z)); return r; }
static inline void Normalize(Vec *out, Vec *v) { float x = v->x, y = v->y, z = v->z, len, inv; __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(len) : "f"(x), "f"(y), "f"(z)); __asm__ volatile("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f), "f"(len)); out->x = x * inv; out->y = y * inv; out->z = z * inv; }
static inline float maxf(float a, float b) { float r; __asm__ volatile("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }

void func_001737F0(Obj *obj)
{
    Vec step;
    Vec normal;
    float rate;
    float height;
    float bc, angle, angle2;

    Scale(&step, &obj->x50, obj->xB8);
    Add(&obj->xC->pos, &obj->xC->pos, &step);
    bc = obj->xBC;
    do { angle = fabsf(bc); } while (0);
    if (angle < 1.0471976f) {
        if (Dot(&obj->x80, &obj->x50) > 0.0f) {
            bc = obj->xBC;
            do { angle2 = fabsf(bc); } while (0);
            rate = 0.075f - angle2 * 0.066845074f;
            obj->x50.x = func_00235EA8(obj->x50.x, obj->xC->x50, rate, 0.0001f);
            obj->x50.y = func_00235EA8(obj->x50.y, obj->xC->x54, rate, 0.0001f);
            obj->x50.z = func_00235EA8(obj->x50.z, obj->xC->x58, 0.01f, 0.0001f);
        }
    }
    Normalize(&obj->x50, &obj->x50);
    if (obj->x4 == 2) {
        height = CallGroundProbe(&obj->xC->pos, &normal, 0, 0.0f);
        if (height != 0.0f) {
            height *= maxf(0.3f, normal.z);
            if (height < 70.0f) {
                obj->xC->pos.z += 0.5f;
            }
        }
    }
    func_00211280(obj->xC);
}
