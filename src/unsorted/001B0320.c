/* UNIT 0x001B0320 0x001B03C8 */
/* func_001B0320: latch steering-side sign from transformed probe.
   obj lives at parent+0x4D40 (see func_001B0268/func_001B0298): x0 = last
   trigger time, x8 = target sign (+/-1), x10 = enable flag, x14 = parent.
   Transforms arg1 by parent->x792C+0x40 into a stack Vec; if |out.x| < 0.55
   do nothing, else stamp x0 and set x8 to -1 (out.x > 0) or +1. */
#include "rider.h"
typedef Vector4 Vec;
typedef struct Obj {
    int x0;
    float x4;
    float x8;
    char padC[4];
    unsigned char x10;
    char pad11[3];
    Rider *x14;
} Obj;

extern int func_00238F48(void);

void func_001B0320(Obj *obj, Vec *arg1)
{
    Vec out;
    float v;
    float m;
    float c;

    if (obj->x14 == 0)
        return;
    if (obj->x10 == 0)
        return;
    func_00221888(&obj->x14->bodyNode->matrix, arg1, &out);
    /* Zero-byte label barriers (cf. src/unsorted/00149BD0.c, 0012F8D8.c):
       reserve FP regs so the load/abs use $f0/$f2, keep v live across the
       0.55f materialisation so abs.s stays before lui/ori/mtc1 (retail
       order) and the constant lands in $f1. Labels emit no bytes. */
    v = out.x;
    __asm__ volatile ("Lm_1B0320:" : "=f" (m));
    __asm__ volatile ("Lc_1B0320:" : "=f" (c));
    m = __builtin_fabsf(v);
    c = 0.55f;
    __asm__ volatile ("Lkeepv_1B0320:" : : "f" (v));
    if (m < c)
        return;
    obj->x0 = func_00238F48();
    if (out.x > 0.0f)
        obj->x8 = -1.0f;
    else
        obj->x8 = 1.0f;
}
