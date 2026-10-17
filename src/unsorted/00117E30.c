/* UNIT 0x00117E30 0x00117ED0 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

static inline void SubVector(Vec4 *out, Vec4 *a, Vec4 *b) {
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline float InnerProduct(Vec4 *a, Vec4 *b) {
    float result;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(result) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return result;
}

/* Nonzero if the projection of point onto the segment start->end lies within it (10% slack). */
int func_00117E30(Vec4 *point, Vec4 *start, Vec4 *end) {
    Vec4 dir;
    Vec4 rel;
    float dot, lengthSq;
    float lo, hi;

    SubVector(&dir, end, start);
    SubVector(&rel, point, start);
    dot = InnerProduct(&dir, &rel);
    lengthSq = InnerProduct(&dir, &dir);
    lo = lengthSq * -0.1f;
    hi = lengthSq * 1.1f;
    if (lo < dot && dot < hi) {
        return 1;
    }
    return 0;
}
