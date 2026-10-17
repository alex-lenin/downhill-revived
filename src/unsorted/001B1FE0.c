/* UNIT 0x001B1FE0 0x001B20B0 */
typedef struct Vec4 { float x,y,z,w; } Vec4 __attribute__((aligned(16)));
typedef struct Obj {
    char pad0[0xA0];
    Vec4 xA0;
    char padB0[0x100];
    Vec4 x1B0;
    char pad1C0[0x100];
    Vec4 x2C0;
} Obj;

extern void func_001B16C8(Vec4 *out, Vec4 *a, Vec4 *b, Vec4 *normal);
extern void func_002224E0(Vec4 *matrix, float *out);

static inline void SubVector(Vec4 *out, Vec4 *a, Vec4 *b) {
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline void OuterProduct(Vec4 *out, Vec4 *a, Vec4 *b) {
    __asm__ volatile ("lqc2 $vf11, %1\n\tlqc2 $vf12, 0(%2)\n\tvopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0" : "=m"(*out) : "m"(*a), "r"(b));
}

static inline void Normalize(Vec4 *out, Vec4 *in) {
    float len;
    float inv;
    float x;
    float y;
    float z;
    int bits;

    x = in->x;
    y = in->y;
    z = in->z;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits)
                      : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
}

void func_001B1FE0(Obj *obj, float *out) {
    Vec4 edge0;
    Vec4 edge1;
    Vec4 cross;
    Vec4 normal;
    Vec4 matrix[4];

    SubVector(&edge0, &obj->x1B0, &obj->xA0);
    SubVector(&edge1, &obj->x2C0, &obj->xA0);
    OuterProduct(&cross, &edge0, &edge1);
    Normalize(&normal, &cross);
    func_001B16C8(matrix, &obj->x1B0, &obj->xA0, &normal);
    func_002224E0(matrix, out);
}
