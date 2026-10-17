/* UNIT 0x00222F60 0x002230B0 */
/* Shortest-arc quaternion rotating unit vector from onto unit vector to
 * (xyz = axis, w = cos(angle/2)); uses inline asm for VU0 and the EE FPU
 * multiply-accumulate / sqrt.s / rsqrt.s instructions. */

/* dst.xyz = a.xyz x b.xyz (VU0 outer product) */
#define CROSS(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,0(%0)" \
                      : : "r"(dst), "r"(a), "r"(b) : "memory")

void func_00222F60(float *quat, float *from, float *to)
{
    float axis[4];
    float dot, root, scale, x, y, z, lengthSq, inv;

    CROSS(quat, from, to);
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot)
                      : "f"(from[0]), "f"(to[0]), "f"(from[1]), "f"(to[1]), "f"(from[2]), "f"(to[2]));
    if (dot > -0.999999f) {
        /* xyz = cross / (2 cos(angle/2)), w = cos(angle/2) */
        scale = 1.0f;
        dot = (dot + scale) * 2.0f;
        __asm__ volatile ("sqrt.s %0,%1" : "=f"(root) : "f"(dot));
        scale = scale / root;
        __asm__ volatile ("lqc2 $vf11,0(%0)\n\tmfc1 $2,%1\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)"
                          : : "r"(quat), "f"(scale) : "$2", "memory");
        quat[3] = root * 0.5f;
        return;
    }
    /* Opposite vectors: rotate 180 degrees about any axis perpendicular to to.
       Zero-byte label: emits no bytes; it keeps the delay-slot pass from copying
       abs.s into the branch above as bc1fl (retail leaves the slot empty). */
    __asm__ volatile ("Lf60a:");
    if (__builtin_fabsf(to[2]) < 0.75f) {
        axis[0] = 0.0f; axis[1] = 0.0f; axis[2] = 1.0f;
    } else {
        axis[0] = 0.0f; axis[1] = 1.0f; axis[2] = 0.0f;
    }
    CROSS(axis, axis, to);
    x = axis[0];
    y = axis[1];
    z = axis[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(lengthSq) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(lengthSq));
    axis[3] = 0.0f;
    quat[0] = x * inv;
    quat[1] = y * inv;
    quat[2] = z * inv;
}
