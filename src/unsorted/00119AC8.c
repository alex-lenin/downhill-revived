/* UNIT 0x00119AC8 0x00119E08 */
#include "include_asm.h"

#ifdef NONMATCHING
/* Non-matching: entry block only: retail loads the start bounds into f1/f2 and 1/1024 into f4 before the stores and copies it to f20 after blez, ours loads K straight into f20; best diff 12/208 words */
#include "soft_double.h"
/* The retail function is entered two words into its range: the code pointer in
   the table at 0x0028398C is 0x00119AD0. The first two words (`addiu $sp,0x10'
   / `nop') are a dead-strip leftover that cc1 cannot emit, so they are provided
   here as data words in .text, with the C body emitted right after them. */
__attribute__((section(".text"))) static const unsigned int prefunc[2] = { 0x27BD0010, 0x00000000 };

/* AI path point ("AiPTool" debug output) */
typedef struct PathPoint {
    float pos[4];
    unsigned int x10;
    unsigned short flags;
    unsigned char numLinks;
    unsigned char x17;
    short links[1];
} PathPoint;

/* Height range of all path points, kept at 0x10070 in the path tool. */
typedef struct HeightBounds {
    float max;
    float min;
    float scale;          /* (max - min) / 1024 */
    PathPoint *first;
    int count;
} HeightBounds;

typedef struct PathTool {
    char pad0[0x1004C];
    int numPoints;
    char pad10050[0x20];
    HeightBounds bounds;
} PathTool;

extern PathPoint *func_001177F8(PathTool *tool, int index);
extern void func_001179D0(HeightBounds *bounds);
extern void func_001181F8(HeightBounds *bounds, int index);
extern void func_00254D08(const char *fmt, ...);
extern char D_00288A78[]; /* "AiPTool: point %d, at %f %f %f has %d paths\n" */
extern char D_00288AA8[]; /* "AiPTool:  Point %d has %d paths\n" */
extern char D_00288AD0[]; /* "AiPTool: Point %d and %d are ONLY %f distance apart\n" */
extern float D_0029C128[]; /* -FLT_MAX */
extern float D_0029C12C[]; /* FLT_MAX */

/* delta.xyz = a.xyz - b.xyz (VU0 macro mode) */
static inline void PathPointSub(float (*out)[4], PathPoint *a, PathPoint *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

/* sqrt(x*x + y*y + z*z) with the EE FPU accumulator */
static inline float Length3(float x, float y, float z)
{
    float length;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(length) : "f"(x), "f"(y), "f"(z));
    return length;
}

void func_00119AC8_body(PathTool *tool)
{
    float delta[4] __attribute__((aligned(16)));
    HeightBounds *bounds;
    HeightBounds *after;
    PathPoint *point;
    PathPoint *first;
    PathPoint *other;
    int count;
    int i;
    int j;
    int next;
    int linked;
    double x;
    double y;
    double z;
    float distance;
    float highZ;
    float lowZ;
    float initialMax;
    float initialMin;
    float tmp;

    bounds = &tool->bounds;
    count = tool->numPoints;
    func_001179D0(bounds);
    first = func_001177F8(tool, 0);
    initialMax = D_0029C128[0];
    initialMin = D_0029C12C[0];
    tmp = initialMax;
    bounds->scale = tmp - initialMin;
    bounds->first = first;
    bounds->count = count;
    bounds->max = initialMax;
    bounds->min = initialMin;

    /* Height range of all points */
    i = 0;
    for (; i < count; i++) {
        point = func_001177F8(tool, i);
        highZ = point->pos[2];
        if (bounds->max < highZ) {
            bounds->max = highZ;
            bounds->scale = (highZ - bounds->min) / 1024.0f;
        }
        lowZ = point->pos[2];
        if (lowZ < bounds->min) {
            bounds->min = lowZ;
            bounds->scale = (bounds->max - lowZ) / 1024.0f;
        }
    }
    after = &tool->bounds;
    if (after->max < after->min) {
        /* no points */
        after->max = 100.0f;
        after->scale = 100.0f / 1024.0f;
        after->min = 0.0f;
    } else {
        after->max += 30.0f;
        after->min -= 30.0f;
        after->scale = (after->max - after->min) / 1024.0f;
    }

    /* Register every point and report suspicious ones */
    for (i = 0; i < count; i = next) {
        next = i + 1;
        func_001181F8(&tool->bounds, i);
        point = func_001177F8(tool, i);
        if (point->numLinks == 0 && next < count && !((point->flags >> 5) & 1)) {
            x = func_001FD300(point->pos[0]);
            y = func_001FD300(point->pos[1]);
            z = func_001FD300(point->pos[2]);
            func_00254D08(D_00288A78, i, x, y, z, point->numLinks);
        }
        if (point->numLinks >= 5U)
            func_00254D08(D_00288AA8, i, point->numLinks);
        for (j = 0; j < point->numLinks; j++) {
            other = func_001177F8(tool, point->links[j]);
            PathPointSub(&delta, point, other);
            if (Length3(delta[0], delta[1], delta[2]) < 10.0f) {
                linked = point->links[j];
                distance = Length3(delta[0], delta[1], delta[2]);
                func_00254D08(D_00288AD0, i, linked, func_001FD300(distance));
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_00119AC8);
#endif
