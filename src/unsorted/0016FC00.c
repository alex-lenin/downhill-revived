/* UNIT 0x0016FC00 0x0016FF30 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec x10;
    char pad20[0x40 - 0x20];
    Vec x40[4];
} Body;

typedef struct Shadow {
    char pad0[0x4];
    unsigned char x4;
    char pad5[0x10 - 0x5];
    Vec x10;
    char pad20[0x40 - 0x20];
    Vec x40[4];
} Shadow;

typedef struct Obj {
    char pad0[0x10];
    Vec x10;
    char pad20[0x1D0 - 0x20];
    char x1D0[0x20];
    Body *x1F0;
    Shadow *x1F4;
    char pad1F8[0x4];
    unsigned long **x1FC;
    char pad200[0x220 - 0x200];
    float x220;
    float x224;
    char pad228[0x234 - 0x228];
    unsigned char x234;
} Obj;

extern char D_002AF2C0[];
extern int func_0011C0D0(void *a, Vec *pos, int b, float radius);
#if defined(__mips__)
extern float func_00211980(Vec *pos, void *p, int a, float b);
#endif
extern void func_00221888(Vec *m, Vec *in, Vec *out);
extern void func_0016FF30(Obj *obj, void *p, int a, int b);
extern void func_00211280(Body *body);
extern int func_00238F30(void);
extern void func_00209248(Body *body, Vec *pos, Vec *m, float scale);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_0016FC00(Obj *obj, int arg)
{
    Vec temp;
    Vec out;
    Vec out2;
    unsigned long **parts;
    int i;
    float height;
    float diff;
    Body *body;
    unsigned long bit57;

    if (obj->x234 == 0 && arg == 0) {
        if (func_0011C0D0(D_002AF2C0, &obj->x1F0->x10, 1, 150.0f) == 0) {
            goto end;
        }
    }

    parts = obj->x1FC;
    for (i = 0; i < 10; i++) {
        if (parts[i] != 0) {
            *parts[i] &= 0xFDFFFFFFFFFFFFFF;
        }
    }
    height = CallGroundProbe(&obj->x1F0->x10, obj->x1D0, 0, 15.0f);
    bit57 = (unsigned long)1 << 57;
    parts = obj->x1FC;
    for (i = 0; i < 10; i++) {
        if (parts[i] != 0) {
            *parts[i] = (*parts[i] & 0xFDFFFFFFFFFFFFFF) | bit57;
        }
    }

    if (obj->x1F4 != 0 && obj->x1F4->x4 != 0) {
        temp.x = 0.0f;
        temp.y = 0.0f;
        temp.z = 0.05f;
        __asm__ volatile ("lqc2 $vf11, %1\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(out) : "m"(temp), "r"(obj->x1F0->x40));
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(obj->x1F4->x10) : "r"(&obj->x1F0->x10), "r"(&out));
    }

    temp.x = 0.0f;
    temp.y = 0.0f;
    temp.z = height - 1.0f;
    func_00221888(obj->x1F0->x40, &temp, &out);
    diff = out.z - obj->x220;
    if (arg != 0 || diff < 0.03f || diff < obj->x224) {
        temp.x = 0.0f;
        temp.y = 0.0f;
        temp.z = diff;
        __asm__ volatile ("lqc2 $vf11, %1\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(out2) : "m"(temp), "r"(obj->x1F0->x40));
        obj->x1F0->x10.z -= out2.z;
        obj->x224 = 0.0f;
        func_0016FF30(obj, obj->x1D0, arg, 1);
    } else {
        obj->x224 += 0.03f;
        obj->x1F0->x10.z -= obj->x224;
        func_0016FF30(obj, obj->x1D0, 0, 0);
    }
    COPY_VECTOR(obj->x10, &obj->x1F0->x10);
    func_00211280(obj->x1F0);
    body = obj->x1F0;
    func_00209248(obj->x1F0, &body->x10, body->x40, 1.0f / (float)func_00238F30());

end:
    if (obj->x1F4 != 0 && obj->x1F4->x4 != 0) {
        __asm__ volatile (
            "lq $8, 0x0(%4)\n\t"
            "lq $9, 0x10(%4)\n\t"
            "lq $10, 0x20(%4)\n\t"
            "lq $11, 0x30(%4)\n\t"
            "sq $8, %0\n\t"
            "sq $9, %1\n\t"
            "sq $10, %2\n\t"
            "sq $11, %3"
            : "=m"(obj->x1F4->x40[0]), "=m"(obj->x1F4->x40[1]), "=m"(obj->x1F4->x40[2]), "=m"(obj->x1F4->x40[3])
            : "r"(obj->x1F0->x40)
            : "$8", "$9", "$10", "$11");
    }
}
