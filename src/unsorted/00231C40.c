/* UNIT 0x00231C40 0x00231C80 */
typedef struct Vec4 { float x, y, z, w; } __attribute__((aligned(16))) Vec4;
typedef struct Obj { char pad0[0x38]; Vec4 *x38; } Obj;

extern void func_00231C20(Obj *obj, Vec4 *v);

/* Rotate v by the 3x3 matrix at obj->x38 and pass the result on */
void func_00231C40(Obj *obj, Vec4 *v)
{
    Vec4 rotated;

    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "lqc2 $vf13, 0x10(%2)\n\t"
        "lqc2 $vf14, 0x20(%2)\n\t"
        "vmulax.xyz ACC, $vf12, $vf11x\n\t"
        "vmadday.xyz ACC, $vf13, $vf11y\n\t"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
        "sqc2 $vf11, %0"
        : "=m"(rotated) : "r"(v), "r"(obj->x38) : "memory");
    func_00231C20(obj, &rotated);
}
