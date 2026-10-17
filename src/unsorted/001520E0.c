/* UNIT 0x001520E0 0x00152108 .sdata=0x29C654:0x29C658 */
typedef struct Obj {
    char pad0[0x10];
    float x10[4];
} Obj;

float D_0029C654 = 0.0f;

/* out.xyz = obj->x10.xyz * D_0029C654 (libvu0 sceVu0ScaleVector pattern) */
void func_001520E0(Obj *obj, int unused, float *out)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tmfc1 $2, %1\n\tqmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0x0(%2)"
                      : : "r"(obj->x10), "f"(D_0029C654), "r"(out) : "$2", "memory");
}
