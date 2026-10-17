/* UNIT 0x001572B0 0x00157400 */
#include "track_projection.h"
#if defined(__mips__)
#define CallTrackProjection func_00119180
#else
#define CallTrackProjection ProjectTrackDiscard
#endif
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Rider {
    char pad0[0x7A68];
    unsigned char x7A68;
} Rider;

typedef struct Obj {
    Rider *x0;
    char pad4[0x4C];
    char x50[0x40];
} Obj;

extern char D_0029EA20[];

extern void func_00221F50(Vec4 *m, void *src);
#if defined(__mips__)
extern void func_00119180(void *list, Vec4 *a, Vec4 *b, void *c, int flag);
#endif
#if defined(__mips__)
extern float func_00211980(Vec4 *pos, Vec4 *normal, int a, float range);
#endif

/* pushes pos out of the sphere (radius + 4) around center; returns 1 if it was outside */
int func_001572B0(Obj *obj, Vec4 *pos, Vec4 *center, float radius)
{
    Vec4 m[4];
    Vec4 d;
    float dist;
    float limit;
    float height;
    int outside = 1;
    int flag;

    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
        : "=m" (d) : "r" (pos), "r" (center));
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
        : "=&f" (dist) : "f" (d.x), "f" (d.y), "f" (d.z));
    limit = radius + 4.0f;
    if (dist < limit) {
        outside = 0;
        func_00221F50(m, obj->x50);
        __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf13\n\t"
                          "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, 0(%0)"
            : : "r" (pos), "r" (&m[1]), "f" (limit) : "$2", "memory");
        flag = 0;
        if (obj->x0 != 0) {
            flag = obj->x0->x7A68 != 0;
        }
        CallTrackProjection(D_0029EA20, pos, pos, obj->x50, flag);
        height = CallGroundProbe(pos, &d, 0, 5.0f);
        if (height != 0.0f && height < 300.0f) {
            pos->z -= height;
        }
    }
    return outside;
}
