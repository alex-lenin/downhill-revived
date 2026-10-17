/* UNIT 0x001B3858 0x001B3AE0 */
#include "camera.h"
#include "rider.h"
typedef Vector4 Vec;

typedef RiderCamera Obj;

extern Vec D_007685A0[];


static inline void sub(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile("lqc2 $vf11,0(%1)\n\t"
                     "lqc2 $vf12,0(%2)\n\t"
                     "vsub.xyz $vf11,$vf11,$vf12\n\t"
                     "sqc2 $vf11,%0"
                     : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void cross(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile("lqc2 $vf11,0(%1)\n\t"
                     "lqc2 $vf12,0(%2)\n\t"
                     "vopmula.xyz ACC,$vf11,$vf12\n\t"
                     "vopmsub.xyz $vf11,$vf12,$vf11\n\t"
                     "sqc2 $vf11,%0"
                     : "=m"(*dst) : "r"(a), "r"(b));
}

/* Sony-style normalization (as in the matched 001304F8 and 00137208 units). The unused
 * output only adds references to the inputs; retail's register allocation needs it. */
static inline float normalize(Vec *vector)
{
    float length, inverse, unused;

    __asm__ volatile("mula.s %3,%3\n\t"
                     "madda.s %4,%4\n\t"
                     "madd.s %2,%5,%5\n\t"
                     "sqrt.s %2,%2\n\t"
                     "lui $3,0x3f80\n\t"
                     "mtc1 $3,%0\n\t"
                     "nop\n\t"
                     "div.s %0,%0,%2"
                     : "=&f"(inverse), "=f"(unused), "=&f"(length)
                     : "f"(vector->x), "f"(vector->y), "f"(vector->z)
                     : "$3");
    vector->x *= inverse;
    vector->y *= inverse;
    vector->z *= inverse;
    return length;
}

#define ROOT(out, value) \
    __asm__ volatile("sqrt.s %0,%1" : "=&f"(out) : "f"(value))

static inline void transform(Vec *dst, Vec *matrix)
{
    __asm__ volatile("lqc2 $vf11,0(%1)\n\t"
                     "lqc2 $vf12,0(%2)\n\t"
                     "lqc2 $vf13,16(%2)\n\t"
                     "lqc2 $vf14,32(%2)\n\t"
                     "vmulax.xyz ACC,$vf12,$vf11x\n\t"
                     "vmadday.xyz ACC,$vf13,$vf11y\n\t"
                     "vmaddz.xyz $vf11,$vf14,$vf11z\n\t"
                     "sqc2 $vf11,%0"
                     : "=m"(*dst) : "r"(dst), "r"(matrix));
}

static inline void add(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile("lqc2 $vf11,0(%1)\n\t"
                     "lqc2 $vf12,0(%2)\n\t"
                     "vadd.xyz $vf11,$vf11,$vf12\n\t"
                     "sqc2 $vf11,%0"
                     : "=m"(*dst) : "r"(a), "r"(b));
}

void func_001B3858(Obj *obj)
{
    Vec basis[3];
    Vec spare; /* unused stack slot, keeps midpoint at sp+0x40 */
    Vec midpoint;
    Vec *point, *previous;
    float length, radius0Squared, radius1Squared, radius2;
    float projection, height, halfHeight, center, otherLength;
    float radius3Squared, otherProjection, otherHeight;
    int index = func_001B1C08(obj->xC0);

    point = &D_007685A0[index];
    previous = (Vec *)((char *)(D_007685A0 - 1) + index * 16);
    sub(&basis[1], point, previous);
    length = normalize(&basis[1]);
    sub(&basis[2], &obj->x20, previous);
    cross(&basis[0], &basis[1], &basis[2]);
    normalize(&basis[0]);
    cross(&basis[2], &basis[0], &basis[1]);
    normalize(&basis[2]);

    radius0Squared = obj->x570;
    radius1Squared = obj->x550;
    radius2 = obj->x530;
    radius0Squared *= radius0Squared;
    radius1Squared *= radius1Squared;
    obj->x0.x = 0;
    projection = (length * length + radius0Squared - radius1Squared) / (length + length);
    obj->x0.y = projection;
    radius0Squared -= projection * projection;
    ROOT(height, radius0Squared);
    length -= projection;
    obj->x0.z = height;
    halfHeight = height * 0.5f;
    center = projection + length * 0.5f;
    midpoint.x = 0;
    midpoint.z = halfHeight;
    midpoint.y = center;
    ROOT(otherLength, length * length + height * height);

    radius3Squared = obj->x5A0 * obj->x5A0;
    otherProjection = (otherLength * otherLength + radius3Squared - radius2 * radius2) /
                      (otherLength + otherLength);
    ROOT(otherHeight, radius3Squared - otherProjection * otherProjection);
    obj->x30.x = 0;
    obj->x30.y = center + (otherHeight * height) / otherLength;
    obj->x30.z = halfHeight + (otherHeight * length) / otherLength;
    transform(&obj->x0, basis);
    add(&obj->x0, &obj->x0, previous);
    transform(&obj->x30, basis);
    add(&obj->x30, &obj->x30, previous);
}
