/* UNIT 0x00232A38 0x00232A80 */
typedef struct {
    char pad0[0x10]; float x10[4];
    char pad20[0x14]; float *x34; float *x38;
    char pad3c[0x14]; float x50[4];
} Obj;
void func_00232A38(Obj *obj) {
    float *dest = obj->x50;
    float *matrix = obj->x38;
    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tlqc2 $vf13, 0x10(%1)\n\tlqc2 $vf14, 0x20(%1)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, 0x50(%2)" : : "r"(obj->x10), "r"(matrix), "r"(obj) : "memory");
    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0x50(%2)" : : "r"(dest), "r"(obj->x34), "r"(obj) : "memory");
}
