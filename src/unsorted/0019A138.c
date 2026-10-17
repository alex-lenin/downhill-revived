/* UNIT 0x0019A138 0x0019A278 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundHeightBetween
#endif
typedef float Vec[4] __attribute__((aligned(16)));
typedef struct { char pad[0x28]; char *x28; } Obj;
extern Vec D_0043C820;
extern void func_00231BD8(void *);
extern void func_00231A08(void *, void *);
extern void func_00221F50(void *, void *);
#if defined(__mips__)
extern float func_00211980(void *, float, void *, int);
#endif
extern void func_00231A78(void *, void *);
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_0019A138(Obj *obj) {
    Vec initial;
    struct { Vec right, up, forward, last; } matrix;
    Vec rotation;
    Vec normal;
    float distance;
    float *n;
    Vec *fwd;
    char *target;
    func_00231BD8(obj->x28 + 0x40C0);
    COPY_VECTOR(initial, D_0043C820);
    func_00231A08(obj->x28 + 0x40C0, initial);
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(rotation));
    rotation[2] = D_0043C820[3] * 0.017453292f;
    func_00221F50(&matrix, rotation);
    n = normal;
    distance = CallGroundProbe(D_0043C820, 7.0f, n, 0);
    if (distance == 0.0f || distance > 100.0f) {
        normal[0] = 0.0f; normal[1] = 0.0f; n[2] = 1.0f;
    }
    target = obj->x28;
    fwd = &matrix.forward;
    normal[3] = 0.0f;
    target += 0x40C0;
    COPY_VECTOR(matrix.forward, normal);
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%2)\n\t"
        "lqc2 $vf12, 0x0(%3)\n\t"
        "vopmula.xyz ACC, $vf11, $vf12\n\t"
        "vopmsub.xyz $vf11, $vf12, $vf11\n\t"
        "sqc2 $vf11, %0\n\t"
        "lqc2 $vf11, 0x0(%3)\n\t"
        "lqc2 $vf12, 0x0(%4)\n\t"
        "vopmula.xyz ACC, $vf11, $vf12\n\t"
        "vopmsub.xyz $vf11, $vf12, $vf11\n\t"
        "sqc2 $vf11, %1"
        : "=m"(matrix.right), "=m"(matrix.up)
        : "r"(matrix.up), "r"(fwd), "r"(matrix.right)
        : "$2", "memory");
    func_00231A78(target, &matrix);
}
