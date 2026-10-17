/* UNIT 0x0021F0A0 0x0021F1B8 */
#include "gs_packet.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef GsDisplayContext GSContext;

extern u128 D_00660200[];
extern void func_0021E4E0(int v);

#define UNCACHED(p) ((unsigned int)(p) & 0x0FFFFFFF | 0x30000000)

void func_0021F0A0(int ctx, int which, int big) {
    GSContext *c = (GSContext *)UNCACHED(ctx);
    int nloop = big ? 0x16 : 0x10;
    int *dst;
    unsigned int dmatag;
    u128 *src;
    u128 *out;
    int i;

    c->pkt[1].tag.b.nloop = nloop;
    c->pkt[0].tag.b.nloop = nloop;
    dst = (int *)UNCACHED(D_00660200);
    dmatag = big ? 0x70000017 : 0x70000011;
    dst[0] = dmatag;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    ctx = (int)(!which ? &c->pkt[0] : &c->pkt[1]);
    src = (u128 *)UNCACHED(ctx);
    out = (u128 *)dst + 1;
    for (i = 0x16; i >= 0; i--) {
        *out++ = *src++;
    }
    *(volatile int *)0x1000A020 = 0;
    *(volatile int *)0x1000A030 = (unsigned int)dst & 0x0FFFFFFF;
    *(volatile int *)0x1000E010 = 4;
    *(volatile int *)0x1000A000 = 0x145;
    func_0021E4E0(which);
}
