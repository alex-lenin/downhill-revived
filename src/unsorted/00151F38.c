/* UNIT 0x00151F38 0x00151F60 .sdata=0x29C650:0x29C654 */
typedef struct Obj {
    char pad0[0x10];
    float x10[4];
} Obj;

float D_0029C650 = 0.1f;

/* out.xyz = obj->x10.xyz * D_0029C650 (libvu0 sceVu0ScaleVector pattern) */
void func_00151F38(Obj *obj, int unused, float *out)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tmfc1 $2, %1\n\tqmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0x0(%2)"
                      : : "r"(obj->x10), "f"(D_0029C650), "r"(out) : "$2", "memory");
}
