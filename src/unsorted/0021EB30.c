/* UNIT 0x0021EB30 0x0021F000 */
#include "gs_packet.h"
typedef unsigned int u128 __attribute__((mode(TI)));

#define U8(p,o)   (*(unsigned char *)((char *)(p) + (o)))
#define U32(p,o)  (*(unsigned int *)((char *)(p) + (o)))
#define U64(p,o)  (*(GsWord64 *)((char *)(p) + (o)))
#define U128(p,o) (*(u128 *)((char *)(p) + (o)))

extern void func_0021EAF0(void *, int, int, int, short *, short *, short *);
extern int func_00243680(void *, short, short, short, short, short);
extern int func_002439F8(void *, short, short, short, short, short);
extern int func_00244940(void *, short, short, short, short, short);
extern int sceGsSetDefClear(void *, int, int, int, int, int, int, int, int, int, int);

void func_0021EB30(void *arg0, short arg1, short arg2, short arg3,
                   short arg4, short arg5, short arg6)
{
    short fb0;
    short fb1;
    short zbuf;
    GsWord64 v18;
    GsWord64 display2;
    GsDispFb frame;
    GsDispFb frame2;
    GsPmode pmode;
    GsWord64 packed;
    GsWord64 xy1;
    GsWord64 xy2;
    int cx;
    int cy;

    func_0021EAF0(arg0, arg2, arg3, arg5, &fb0, &fb1, &zbuf);
    func_00243680(arg0, arg1, arg2, arg3, 0, 0);

    ((GsDispFb *)((char *)arg0 + 0x10))->b.fbp = fb0;
    ((GsDisplay *)((char *)arg0 + 0x30))->raw = ((GsDisplay *)((char *)arg0 + 0x18))->raw;
    ((GsDisplay *)((char *)arg0 + 0x30))->b.dh = (((unsigned int)(U64(arg0, 0x18) >> 44) & 0x7ff) - 1) & 0x7ff;
    ((GsDispFb *)((char *)arg0 + 0x28))->raw = ((GsDispFb *)((char *)arg0 + 0x10))->raw;
    ((GsDispFb *)((char *)arg0 + 0x28))->b.dby = 1;
    ((GsDisplay *)((char *)arg0 + 0x30))->b.dx = ((U32(arg0, 0x18) & 0xfff) + 4) & 0xfff;
    ((GsPmode *)((char *)arg0 + 0))->b.alp = 0x80;
    ((GsPmode *)((char *)arg0 + 0))->b.en1 = 1;
    ((GsPmode *)((char *)arg0 + 0))->b.en2 = 1;

    func_002439F8((char *)arg0 + 0x90, arg1, arg2, arg3, arg4, arg5);
    func_00244940((char *)arg0 + 0x110, arg1, arg2, arg3, arg4, arg5);

    packed = ((GsWord64)(arg5 & 0xf) << 24);
    ((GsFrame *)((char *)arg0 + 0x90))->b.fbp = fb0;
    ((GsFrame *)((char *)arg0 + 0x110))->b.fbp = fb0;
    xy1 = (GsWord64)zbuf | packed;
    U64(arg0, 0xa0) = arg4 ? xy1 : (xy1 | ((GsWord64)0x8000 << 17));
    U64(arg0, 0x120) = arg4 ? xy1 : (xy1 | ((GsWord64)0x8000 << 17));

    ((GifTag *)((char *)arg0 + 0x80))->wide = 0;
    ((GifTag *)((char *)arg0 + 0x80))->b.nloop = 0x16;
    ((GifTag *)((char *)arg0 + 0x80))->b.eop = 1;
    ((GifTag *)((char *)arg0 + 0x80))->b.nreg = 1;
    ((GifTag *)((char *)arg0 + 0x80))->b.regs0 = 0xe;

    cx = 0x800 - (arg2 >> 1);
    cy = 0x800 - (arg3 >> 1);
    sceGsSetDefClear((char *)arg0 + 0x190, arg4, cx, cy, arg2, arg3, 0, 0, 0, 0, 0);

    func_00243680((char *)arg0 + 0x40, arg1, arg2, arg3, 0, 0);

    ((GsDispFb *)((char *)arg0 + 0x50))->b.fbp = fb1;
    ((GsDisplay *)((char *)arg0 + 0x70))->raw = ((GsDisplay *)((char *)arg0 + 0x58))->raw;
    ((GsDisplay *)((char *)arg0 + 0x70))->b.dh = (((unsigned int)(U64(arg0, 0x58) >> 44) & 0x7ff) - 1) & 0x7ff;
    ((GsDispFb *)((char *)arg0 + 0x68))->raw = ((GsDispFb *)((char *)arg0 + 0x50))->raw;
    ((GsDispFb *)((char *)arg0 + 0x68))->b.dby = 1;
    ((GsDisplay *)((char *)arg0 + 0x70))->b.dx = ((U32(arg0, 0x58) & 0xfff) + 4) & 0xfff;
    ((GsPmode *)((char *)arg0 + 0x40))->b.alp = 0x80;
    ((GsPmode *)((char *)arg0 + 0x40))->b.en1 = 1;
    ((GsPmode *)((char *)arg0 + 0x40))->b.en2 = 1;

    func_002439F8((char *)arg0 + 0x200, arg1, arg2, arg3, arg4, arg5);
    func_00244940((char *)arg0 + 0x280, arg1, arg2, arg3, arg4, arg5);

    ((GsFrame *)((char *)arg0 + 0x200))->b.fbp = fb1;
    ((GsFrame *)((char *)arg0 + 0x280))->b.fbp = fb1;
    xy2 = (GsWord64)zbuf | packed;
    U64(arg0, 0x210) = arg4 ? xy2 : (xy2 | ((GsWord64)0x8000 << 17));
    U64(arg0, 0x290) = arg4 ? xy2 : (xy2 | ((GsWord64)0x8000 << 17));

    ((GifTag *)((char *)arg0 + 0x1f0))->wide = 0;
    ((GifTag *)((char *)arg0 + 0x1f0))->b.nloop = 0x16;
    ((GifTag *)((char *)arg0 + 0x1f0))->b.eop = 1;
    ((GifTag *)((char *)arg0 + 0x1f0))->b.nreg = 1;
    ((GifTag *)((char *)arg0 + 0x1f0))->b.regs0 = 0xe;

    sceGsSetDefClear((char *)arg0 + 0x300, arg4, cx, cy, arg2, arg3, 0, 0, 0, 0, 0);
}
