/* UNIT 0x0016A488 0x0016A788 .sdata=0x29C844:0x29C854 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0016A010_14_func_0016A010 func_0016A010
#else
#define CallScalarBridge0016A010_14_func_0016A010 ScalarBridge0016A010_14
#endif
typedef struct Vec { float x, y, z, w; } Vec;
struct Rider { char pad0[0x38]; Vec *x38; char pad3C[0x84]; Vec xC0; };
struct State { int x0; struct Rider *x4; char pad8[8]; Vec x10, x20, x30, x40; };

int D_0029C844 = 1, D_0029C848 = 1, D_0029C84C = 1;
/* D_0029C86C and D_0029C870 are defined elsewhere; hoist their sizes so the
   assembler uses $gp (retail does). */
__asm__(".extern D_0029C86C, 4\n\t.extern D_0029C870, 4");
extern float D_0029C86C, D_0029C870;


#if defined(__mips__)
extern int func_0016A010(struct State *, float);
#else

#endif

extern int func_0016A1F8(struct State *, float);
extern int func_0016A3E8(struct State *, float);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
/* Same copy, also taking the destination address in a register: retail has
   &s->x10 computed before the first copy. */
#define COPY_VECTOR_TO(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src), "r"(&(dst)) : "$2")

static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
                      : "=f"(r) : "0"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

/* dst = a + b * k */
static inline void AddScaled(Vec *dst, Vec *a, Vec *b, float k) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\t"
                      "vaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b), "f"(k) : "$2");
}

static inline void Normalize(Vec *v) {
    float length, inv, x = v->x, y = v->y, z = v->z;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(length) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(length));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
}

/* dst = src projected onto the plane with normal n, normalized */
static inline void ProjectOnPlane(Vec *dst, Vec *src, Vec *n) {
    COPY_VECTOR(*dst, src);
    AddScaled(dst, dst, n, -Dot(dst, n));
    Normalize(dst);
}

int func_0016A488(struct State *s, Vec *normal, float a, float b) {
    if (!D_0029C844 && !D_0029C848 && !D_0029C84C) return 0;
    COPY_VECTOR_TO(s->x10, normal);
    ProjectOnPlane(&s->x30, s->x4->x38 + 1, &s->x10);
    ProjectOnPlane(&s->x20, s->x4->x38, &s->x10);
    ProjectOnPlane(&s->x40, &s->x4->xC0, &s->x10);
    switch (s->x0) {
    case 0:
        if (D_0029C84C && func_0016A3E8(s, b)) return 3;
        if (D_0029C844 && CallScalarBridge0016A010_14_func_0016A010(s, a)) return 1;
        goto check2;
    case 1:
        if (D_0029C84C) {
            if (func_0016A3E8(s, b)) return 3;
        }
        break;
    case 2:
    check2:
        if (D_0029C848 && func_0016A1F8(s, D_0029C86C)) return 2;
        break;
    case 3:
        if (D_0029C848) {
            int result = func_0016A1F8(s, D_0029C870);
            if (result) return 2;
        }
        break;
    }
    return 0;
}
