/* UNIT 0x001439A8 0x00143A80 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec4 x10;
} Body;

typedef struct Obj {
    char pad0[0xF0];
    Body *xF0;
} Obj;

/* keeps the 4 nearest positions sorted by distance (stored in w) */
void func_001439A8(Obj *self, Obj *other, Vec4 *nearest)
{
    Vec4 d;
    float dist;

    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%2)"
        : : "r" (&other->xF0->x10), "r" (&self->xF0->x10), "r" (&d) : "memory");
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
        : "=&f" (dist) : "f" (d.x), "f" (d.y), "f" (d.z));
    d.w = dist;
    if (dist < nearest[3].w) {
        if (dist < nearest[2].w) {
            nearest[3] = nearest[2];
            if (dist < nearest[1].w) {
                nearest[2] = nearest[1];
                if (dist < nearest[0].w) {
                    nearest[1] = nearest[0];
                    nearest[0] = d;
                } else {
                    nearest[1] = d;
                }
            } else {
                nearest[2] = d;
            }
        } else {
            nearest[3] = d;
        }
    }
}
