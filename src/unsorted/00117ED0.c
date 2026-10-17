/* UNIT 0x00117ED0 0x00118048 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

extern void func_002226C8(float *mat, Vec4 *axis);
extern void func_00221888(float *mat, Vec4 *in, Vec4 *out);

static inline void SubVector(Vec4 *out, Vec4 *a, Vec4 *b) {
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline float InnerProduct(Vec4 *a, Vec4 *b) {
    float result;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(result) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return result;
}

/* Distance of point c from the line a->b in the XY plane; with sign set, negative
   when c lies on the negative-x side in the line's local frame. */
float func_00117ED0(Vec4 *a, Vec4 *b, Vec4 *c, int sign)
{
    Vec4 dir;
    Vec4 rel;
    Vec4 local;
    float mat[16] __attribute__((aligned(16)));
    float ax, ay, dx, dy, ex, ey, cross, dist, lengthSq, inv;

    ax = a->x;
    ay = a->y;
    ey = c->y - ay;
    dx = b->x - ax;
    ex = c->x - ax;
    dy = b->y - ay;
    /* the do/while (0) keeps the abs.s and the epsilon load after the cross product */
    do {
        cross = ey * dx - ex * dy;
    } while (0);
    if (__builtin_fabsf(dx) > 1e-10f || __builtin_fabsf(dy) > 1e-10f) {
        dist = (cross * cross) / (dx * dx + dy * dy);
        __asm__ volatile ("sqrt.s %0, %0" : "+f"(dist));
        if (sign) {
            SubVector(&dir, b, a);
            lengthSq = InnerProduct(&dir, &dir);
            __asm__ volatile ("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f), "f"(lengthSq));
            dir.x = dir.x * inv;
            dir.y = dir.y * inv;
            dir.z = dir.z * inv;
            func_002226C8(mat, &dir);
            SubVector(&rel, c, a);
            func_00221888(mat, &rel, &local);
            if (local.x < 0.0f) {
                dist = -dist;
            }
        }
    } else {
        dist = 0.0f;
    }
    return dist;
}
