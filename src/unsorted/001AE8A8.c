/* UNIT 0x001AE8A8 0x001AE980 */
#include "rider.h"
typedef Vector4 Vec4;

typedef Matrix4 Transform;

typedef SceneNode Target;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

#define CROSS(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0" \
                      : "=m"(dst) : "r"(a), "r"(b))

#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\tsq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
                      : "=m"((dst).row[0]), "=m"((dst).row[1]), \
                        "=m"((dst).row[2]), "=m"((dst).row[3]) : "r"(src) \
                      : "$8", "$9", "$10", "$11")

static inline void Normalize(Vec4 *v)
{
    float x = v->x, y = v->y, z = v->z;
    float length, inv;
    int bits;

    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(length), "=r"(bits)
                      : "f"(x), "f"(y), "f"(z));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
}

void func_001AE8A8(Target *target, Vec4 *dir, Vec4 *up, Transform *out)
{
    Vec4 u;
    Transform m;

    COPY_VECTOR(m.row[1], dir);
    COPY_VECTOR(u, up);
    CROSS(m.row[0], &u, &m.row[1]);
    Normalize(&m.row[0]);
    CROSS(m.row[2], &m.row[1], &m.row[0]);
    m.row[3].x = 0.0f;
    m.row[3].y = 0.0f;
    m.row[3].z = 0.0f;
    m.row[3].w = 1.0f;
    m.row[0].w = 0.0f;
    m.row[1].w = 0.0f;
    m.row[2].w = 0.0f;
    COPY_MATRIX(*out, &m);
}
