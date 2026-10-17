/* UNIT 0x00207E68 0x00207F80 */
typedef struct {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct {
    Vec pos;
    float *x10;
    float *x14;
    float *x18;
    float *x1C;
    float *x20;
    float *x24;
    char pad28[4];
    unsigned char x2C;
} Obj;

extern void func_00222040(void *mtx, float *rot);

static inline float GetValue(float *p) {
    float v = 0.0f;
    if (p) {
        v = *p;
    }
    return v;
}

void func_00207E68(Obj *obj, void *mtx, Vec *out) {
    float rot[3];
    Vec tmp;
    float x, y, z;

    if (obj->x2C) {
        x = -GetValue(obj->x10);
        y = -GetValue(obj->x14);
        z = -GetValue(obj->x18);
        rot[0] = x;
        rot[1] = y;
        rot[2] = z;
        func_00222040(mtx, rot);
        x = GetValue(obj->x1C);
        y = GetValue(obj->x20);
        z = GetValue(obj->x24);
        out->x = x;
        out->y = y;
        out->z = z;
        __asm__ volatile (
            "lqc2       $vf11, 0x0(%0)\n"
            "\tlqc2       $vf12, 0x0(%1)\n"
            "\tvadd.xyz   $vf11, $vf11, $vf12\n"
            "\tsqc2       $vf11, 0x0(%0)"
            : : "r"(out), "r"(obj) : "memory");
        __asm__ volatile (
            "lqc2       $vf11, 0x0(%1)\n"
            "\tlqc2       $vf12, 0x0(%2)\n"
            "\tlqc2       $vf13, 0x10(%2)\n"
            "\tlqc2       $vf14, 0x20(%2)\n"
            "\tvmulax.xyz ACC, $vf12, $vf11x\n"
            "\tvmadday.xyz ACC, $vf13, $vf11y\n"
            "\tvmaddz.xyz $vf11, $vf14, $vf11z\n"
            "\tsqc2       $vf11, %0"
            : "=m"(tmp) : "r"(obj), "r"(mtx) : "memory");
        __asm__ volatile (
            "lqc2       $vf11, 0x0(%0)\n"
            "\tlqc2       $vf12, 0x0(%1)\n"
            "\tvsub.xyz   $vf11, $vf11, $vf12\n"
            "\tsqc2       $vf11, 0x0(%0)"
            : : "r"(out), "r"(&tmp) : "memory");
    }
}
