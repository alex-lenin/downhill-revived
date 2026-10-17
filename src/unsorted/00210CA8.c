/* UNIT 0x00210CA8 0x00210F20 */
#include "spatial.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vector { u128 q; float f[4]; } Vector;
typedef struct Matrix { Vector v[4]; } Matrix;
typedef SpatialCollider Shape;
typedef SpatialReference Context;
typedef SpatialHit Hit;




extern int func_00213C08(Vector *, Vector *, float, Matrix *, int, Vector *, Vector *, void *, Hit *, int);

int func_00210CA8(Shape *shape, Context *context, float radius, void *start, void *end, float t0, float t1, Matrix4 *matrix, int flags, Hit *hits, int capacity)
{
    Vector a, b;
    Matrix transformed;
    Vector lower, upper, expansion;
    int count;
    unsigned int i;
    Hit *hit;
    Vector *pos, *normal;
    Vector *low, *high;
    int remaining;
    float scale;
    func_00212500(context, start, (Vector4 *)&a, t0);
    func_00212500(context, end, (Vector4 *)&b, t1);
    if (matrix != 0) {
        func_00212488(context, matrix, (Matrix4 *)&transformed, t0);
    } else {
        /* Sony sceVu0CopyMatrix sequence. */
        __asm__ volatile (
            "lq $8, 0x0(%4)
	lq $9, 0x10(%4)
	lq $10, 0x20(%4)
	lq $11, 0x30(%4)
	"
            "sq $8, %0
	sq $9, %1
	sq $10, %2
	sq $11, %3"
            : "=m"(transformed.v[0]), "=m"(transformed.v[1]), "=m"(transformed.v[2]), "=m"(transformed.v[3])
            : "r"(matrix) : "$8", "$9", "$10", "$11");
    }
    count = 0;
    switch (shape->x0 & 0x3F) {
    case 0xA: break;
    case 0x2A: {
        low = &lower;
        high = &upper;
        expansion.f[2] = radius;
        expansion.f[1] = radius;
        expansion.f[0] = radius;
        __asm__ volatile("lqc2 $vf11, 0x0(%2)\n\tlqc2 $vf12, 0x0(%3)\n\tvmini.xyz $vf13, $vf11, $vf12\n\tvmax.xyz $vf14, $vf11, $vf12\n\tsqc2 $vf13, %0\n\tsqc2 $vf14, %1" : "=m"(lower), "=m"(upper) : "r"(&a), "r"(&b) : "memory");
        __asm__ volatile("lqc2 $vf11, 0x0(%0)
	lqc2 $vf12, 0x0(%1)
	vadd.xyz $vf11, $vf11, $vf12
	sqc2 $vf11, %3
	lqc2 $vf11, 0x0(%2)
	lqc2 $vf12, 0x0(%1)
	vsub.xyz $vf11, $vf11, $vf12
	sqc2 $vf11, %4" : : "r"(high), "r"(&expansion), "r"(low), "m"(upper), "m"(lower) : "memory");
        {
            for (i = 0; i < shape->x4; ++i) {
                count += func_00213C08(&a, &b, radius, &transformed, flags, &lower, &upper, shape->xC[i], hits + count, capacity - count);
            }
        }
        if (count > 0) {
            int remap;
            hit = hits;
            remap = count;
            do {
                --remap;
                hit->x6 = shape->x8.map[hit->x6];
                ++hit;
            } while (remap != 0);
        }
        break;
    }
    default: break;
    }
    for (remaining = 0; remaining < count; ++remaining) {
        __asm__ volatile("max.s %0, %1, %2" : "=&f"(scale) : "f"(hits[remaining].x10.w), "f"(0.0f));
        func_002125A8(context, &hits[remaining].x10, &hits[remaining].x10, scale);
        func_00212688(context, &hits[remaining].x20, &hits[remaining].x20, scale);
        hits[remaining].xC = (int)context->actor;
    }
    return count;
}
