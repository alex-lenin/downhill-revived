/* UNIT 0x0012E598 0x0012E780 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Body { char pad0[0x10]; Vec x10; char pad20[0x20]; Vec x40; } Body;
typedef struct Rider {
    char pad0[0x792C];
    Body *x792C;
    char pad7930[0x4C];
    int x797C;
    char pad7980[0xDC];
    int x7A5C;
    char pad7A60[0x170];
} Rider;
typedef struct Global { Rider riders[8]; char pad3DE80[0x501A8 - 0x3DE80]; int x501A8; } Global;
extern Global D_002AF2C0;
extern void func_00221888(Vec *, Vec *, Vec *);
extern float fabsf(float);
#define VSUB(d,a,b) __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(d) : "r"(a), "r"(b))

/* Finds the nearest other rider inside a box around this one; returns 1 if found. */
int func_0012E598(Rider *rider, Vec *out, Rider **found) {
    Vec delta;
    Vec bounds;
    Vec *limits;
    Rider *other;
    int i;
    int offset;
    int hit;
    float nearest;
    float dx, dy, dz;
    float by, bz, az;

    *found = 0;
    i = 0;
    offset = 0;
    nearest = 999999.875f;
    hit = 0;
    for (; i < D_002AF2C0.x501A8; i++, offset += sizeof(Rider)) {
        limits = &bounds;
        if (i != rider->x797C) {
            other = (Rider *)((char *)D_002AF2C0.riders + offset);
            /* the do/while (0) block's loop notes are a scheduling barrier
               that keeps the bounds.z load after fabsf(dz) */
            do {
                VSUB(delta, &other->x792C->x10, &rider->x792C->x10);
                if (rider->x7A5C == 1) {
                    bounds.x = 8.0f; limits->y = 6.0f; limits->z = 6.0f;
                    dx = delta.x;
                } else if (other->x7A5C == 1) {
                    by = 3.0f; bz = 4.0f;
                    bounds.x = 6.0f; limits->y = by; limits->z = bz;
                    dx = delta.x;
                } else {
                    bounds.x = 8.0f; limits->y = 6.0f; limits->z = 6.0f;
                    dx = delta.x;
                }
                dy = delta.y;
                if (!(fabsf(dx) < bounds.x)) goto next;
                if (!(fabsf(dy) < bounds.y)) goto next;
                dz = delta.z;
            } while (0);
            az = fabsf(dz);
            if (az < bounds.z) {
                /* 2D distance from dx, dy; dz is reused for the result */
                __asm__ ("mula.s %1, %1\n\tmadd.s %0, %2, %2\n\tsqrt.s %0, %0" : "=f"(dz) : "f"(dx), "f"(dy), "f"(dz), "f"(dz), "f"(dz), "f"(bounds.z), "f"(bounds.z), "f"(az), "f"(dy));
                if (dz < nearest) {
                    nearest = dz;
                    hit = 1;
                    func_00221888(&rider->x792C->x40, &delta, out);
                    *found = other;
                }
            }
        }
next:;
    }
    return hit;
}
