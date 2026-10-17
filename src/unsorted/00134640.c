/* UNIT 0x00134640 0x00134688 */
typedef struct { float x, y, z, w; } Vector __attribute__((aligned(16)));
struct Obj { char pad0[0x38]; int x38; char pad3C[0x14]; Vector x50; };
void func_00134640(struct Obj *obj, Vector *vector, float *scale, int *value, float *weights) {
    __asm__ volatile ("lq $2, 0(%1)
	sq $2, 0(%0)" : : "r"(vector), "r"(&obj->x50) : "$2", "memory");
    *value = obj->x38;
    weights[0] = 1.0f;
    weights[1] = 0.33333334f;
    weights[2] = 0.33333334f;
    *scale = 3.0f;
}
