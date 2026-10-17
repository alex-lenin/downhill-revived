/* UNIT 0x001A8830 0x001A8918 */
#include "rider.h"
typedef Vector4 Vec;

typedef struct Point {
    Vec pos;
    char pad10[0x9];
    signed char type;
    char pad1A[0x6];
} Point;

typedef struct PointTable {
    char pad0[0x9D0];
    Point points[1];
} PointTable;

typedef struct IdList {
    int count;
    short ids[1];
} IdList;

extern Rider *D_0037ED14[];

/* dst.xyz = a.xyz - b.xyz (VU0 macro mode) */
static inline void VecSub(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b) : "memory");
}

static inline float VecLength(Vec *v)
{
    float length;

    __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0"
                      : "=&f"(length) : "f"(v->x), "f"(v->y), "f"(v->z));
    return length;
}
extern PointTable D_0043F9F0;

int func_001A8830(IdList *list, int *special)
{
    Vec d;
    float dist;
    float best;
    int result;
    int i;
    int id;
    Rider *world;

    best = 1e8f;
    *special = -1;
    i = 0;
    world = D_0037ED14[0];
    result = 0;
    for (; i < list->count; i++) {
        id = list->ids[i];
        if (D_0043F9F0.points[id].type == 0) {
            VecSub(&d, &D_0043F9F0.points[id].pos, &world->bodyNode->position);
            dist = VecLength(&d);
            if (dist < best) {
                result = id;
                best = dist;
            }
        } else if (D_0043F9F0.points[id].type == 1) {
            *special = id;
        } else if (D_0043F9F0.points[id].type == 2) {
            result = list->ids[i];
        }
    }
    return result;
}
