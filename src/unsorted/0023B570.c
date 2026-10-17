/* UNIT 0x0023B570 0x0023B770 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0023B220_32_func_0023B220 func_0023B220
#else
#define CallScalarBridge0023B220_32_func_0023B220 ScalarBridge0023B220_32
#endif
typedef struct Ring {
    unsigned int x0;
    unsigned int x4;
    int x8, xC, x10, x14;
    char pad18[0x28];
    int x40, x44;
} Ring;

extern char D_002993A8[];
extern int func_0025A720(int);
extern void func_0025A700(int);
extern void func_002243E8(void *);
extern void func_0023B1B0(int);
extern int func_0023B0F8(Ring *, unsigned int);

#if defined(__mips__)
extern void func_0023B220(unsigned int, unsigned int, int, int);
#else

#endif


int func_0023B570(Ring *o)
{
    int flags, ringSize, start, consumed, pending, next, total, blocks, index, result;
    int cursor;
    int previous, didAdd, i, remaining, mode;
    didAdd = 0;
    func_0025A720(o->x40);
    if (o->x44 == 0) {
        func_002243E8(D_002993A8);
        return 0;
    }
    func_0023B1B0(5);
    flags = *(volatile unsigned int *)0x1000B400;
    consumed = func_0023B0F8(o, *(volatile unsigned int *)0x1000B410);
    ringSize = o->x8;
    start = o->xC;
    consumed = (consumed + ringSize - start) % ringSize;
    next = (start + consumed) % ringSize;
    pending = o->x10 - consumed;
    o->x10 = pending;
    total = next + pending;
    o->xC = next;
    index = total % ringSize;
    remaining = o->x14;
    blocks = remaining / 2048;
    o->x14 = remaining - (blocks << 11);
    if (blocks > 0) {
        previous = ringSize - 1;
        previous = (total + previous) % ringSize;
        CallScalarBridge0023B220_32_func_0023B220(o->x4 + (previous << 4), o->x0 + (previous << 11), 3, 128);
        didAdd = 1;
    }
    cursor = index;
    for (i = 0; i < blocks; i++) {
        mode = 0;
        if ((i ^ (blocks - 1)) != 0) mode = 3;
        CallScalarBridge0023B220_32_func_0023B220(o->x4 + (cursor << 4), o->x0 + (cursor << 11), mode, 128);
        cursor = (cursor + 1) % o->x8;
    }
    o->x10 += blocks;
    if (o->x10) {
        if (didAdd) flags = (flags & 0x0FFFFFFF) | 0x30000000;
        func_0023B1B0(flags | 0x100);
    }
    func_0025A700(o->x40);
    return 1;
}
