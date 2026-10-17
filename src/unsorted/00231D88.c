/* UNIT 0x00231D88 0x00231E00 */
struct Object { char pad[0x34]; float *x34; float *x38; };
extern int func_00231CC0(struct Object *, float *, float *);
#define ROTATE(out,mat,in) __asm__ volatile ( \
    "lqc2 $vf11,0(%2)\n\t" \
    "lqc2 $vf12,0(%1)\n\t" \
    "lqc2 $vf13,16(%1)\n\t" \
    "lqc2 $vf14,32(%1)\n\t" \
    "vmulax.xyz ACC,$vf12,$vf11x\n\t" \
    "vmadday.xyz ACC,$vf13,$vf11y\n\t" \
    "vmaddz.xyz $vf11,$vf14,$vf11z\n\t" \
    "sqc2 $vf11,%0" : "=m"(out) : "r"(mat), "r"(in) : "memory")
int func_00231D88(struct Object *obj, float *a, float *b) {
    float rotatedA[4] __attribute__((aligned(16)));
    float rotatedB[4] __attribute__((aligned(16)));
    float *matrix = obj->x38;
    ROTATE(rotatedA, matrix, a);
    ROTATE(rotatedB, matrix, b);
    matrix = obj->x34;
    b = rotatedB;
    __asm__ (
        "lqc2 $vf11,0(%1)\n\t"
        "lqc2 $vf12,0(%2)\n\t"
        "vadd.xyz $vf11,$vf11,$vf12\n\t"
        "sqc2 $vf11,%0" : "=m"(*b) : "r"(b), "r"(matrix));
    return func_00231CC0(obj, rotatedA, b);
}
