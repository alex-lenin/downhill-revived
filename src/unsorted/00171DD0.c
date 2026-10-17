/* UNIT 0x00171DD0 0x00171E58 */
#include "path_motion.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Src {
    char pad0[0x10];
    u128 x10;
} Src;

typedef struct Obj {
    char pad0[0x4];
    Src *x4;
    char pad8[0x50 - 0x8];
    u128 x50;
    u128 x60;
    u128 x70;
    char pad80[0xA0 - 0x80];
    char xA0[0x110 - 0xA0];
    char x110[0x1E4 - 0x110];
    int x1E4;
    int x1E8;
    char pad1EC[4];
    int x1F0;
    char pad1F4[4];
    int x1F8;
    int x1FC;
} Obj;

extern void func_00171F10(Obj *);
extern int func_00238F60(void);

extern void func_00171958(Obj *, int);

void func_00171DD0(Obj *obj)
{
    func_00171F10(obj);
    obj->x1F8 += func_00238F60();
    if (obj->x1FC < obj->x1F8) {
        /* quadword copy (sceVu0CopyVector style) */
        __asm__ volatile ("lq $2,0x0(%1)\n\tsq $2,%0" : "=m"(obj->x50) : "r"(&obj->x4->x10) : "$2");
        __asm__ volatile ("sqc2 $vf0,%0" : "=m"(obj->x60));
        __asm__ volatile ("sqc2 $vf0,%0" : "=m"(obj->x70));
        obj->x1E4 = 0;
        obj->x1E8 = 0;
        obj->x1F0 = 0;
        func_001DDA50(obj->xA0, (PathTrack *)obj->x110, 0);
        func_00171958(obj, 0);
    }
}
