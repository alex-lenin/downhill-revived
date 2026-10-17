/* UNIT 0x001BB040 0x001BB520 .rodata=0x28F090:0x28F0B0 */
typedef int s128 __attribute__((mode(TI)));
struct Display { unsigned short x0, x2, x4, x6; char pad[16]; int x18, x1C; };

extern long D_004B2060[];
extern long D_004B21A0[];
extern char D_00513B00[];
extern struct Display *func_0023DF40(int);
extern long *func_001BB520(long *p, int x, int y, int width, int height, int color);
extern long *func_001BB5A0(long *p, int x, int y, int w, int h, int c0, int c1, int c3, int c2);
extern void func_002103D8(void *p, int i, void *packet);

void func_001BB040(void *unused, int mode)
{
    long *buf;
    long *p;
    struct Display *d;
    int x, y;

    if (mode == 2 || mode == 4) {
        p = D_004B21A0;
    } else {
        p = D_004B2060;
    }
    buf = p;
    d = func_0023DF40(0);
    x = d->x18 - (d->x4 >> 1);
    y = d->x1C - (d->x6 >> 1);
    *(s128 *)buf = 0;
    *(int *)buf = 0x60000006;
    buf[2] = ((long)0x8000 << 45) | 0x8001;
    buf[3] = 14;
    buf[4] = ((long)0x8000 << 24) | 0x44;
    buf[5] = 0x42;
    p = buf + 6;
    switch (mode) {
    case 0:
        p = func_001BB520(p, x - 12, y + 0x30, d->x4 - 0x10E, d->x6 - 0x88, 0x28);
        break;
    case 1: {
        int x1, y1;

        x1 = x - 0x41;
        y1 = y + 0xC6;
        p = func_001BB520(p, x1, y1, d->x4 + 0x82, d->x6 - 0xDE, 0x7F);
        p = func_001BB520(p, x1, y1, d->x4 + 0x82, d->x6 - 0xD5, 0x28);
        p = func_001BB520(p, x1, y + 0xD1, d->x4 + 0x82, d->x6 - 0xDE, 0x7F);
        *(int *)buf = 0x6000000E;
        break;
    }
    case 2: {
        int x2, y2, y3, x3;
        int w, rem;
        float fw;

        x2 = x - 0x41;
        y2 = y + 0x2A;
        fw = d->x4 - 0xD2;
        w = fw * 0.9f;
        rem = fw - w;
        y3 = y + 0x9E;
        p = func_001BB520(p, x2, y2, w, d->x6 - 0xDE, 0x7F);
        p = func_001BB520(p, x2, y2, w, d->x6 - 0x6A, 0x28);
        p = func_001BB520(p, x2, y3, w, d->x6 - 0xDE, 0x7F);
        x3 = x2 + w;
        p = func_001BB5A0(p, x3, y2, rem, d->x6 - 0xDE, 0x7F, 0, 0x7F, 0);
        p = func_001BB5A0(p, x3, y2, rem, d->x6 - 0x6A, 0x28, 0, 0x28, 0);
        p = func_001BB5A0(p, x3, y3, rem, d->x6 - 0xDE, 0x7F, 0, 0x7F, 0);
        *(int *)buf = 0x60000029;
        break;
    }
    case 3: {
        int x1, y1;

        x1 = x - 0x41;
        y1 = y + 0xCB;
        p = func_001BB520(p, x1, y1, d->x4 + 0x82, 2, 0x7F);
        p = func_001BB520(p, x1, y1, d->x4 + 0x82, 11, 0x28);
        p = func_001BB520(p, x1, y + 0xD6, d->x4 + 0x82, 2, 0x7F);
        *(int *)buf = 0x6000000E;
        break;
    }
    case 4:
        /* the (long) casts keep gcc from folding the -1/+1 into d->x1C */
        p = func_001BB520(p, d->x18 - 1, d->x1C - (d->x6 >> 1), 2, d->x6, 0x50);
        p = func_001BB520(p, d->x18 + 0x9F, d->x1C - (d->x6 >> 1), 2, d->x6, 0x50);
        p = func_001BB520(p, d->x18 - 0xA1, d->x1C - (d->x6 >> 1), 2, d->x6, 0x50);
        p = func_001BB520(p, d->x18 - 0x140, d->x1C - 1, 0x280, 2, 0x50);
        p = func_001BB520(p, d->x18 - 0x140, d->x1C + (long)(d->x6 / 4 - 1), 0x280, 2, 0x50);
        p = func_001BB520(p, d->x18 - 0x140, d->x1C - (long)(d->x6 / 4 + 1), 0x280, 2, 0x50);
        *(int *)buf = 0x6000001C;
        break;
    }
    if (mode == 2 || mode == 4) {
        func_002103D8(D_00513B00, 0, D_004B21A0);
    } else {
        func_002103D8(D_00513B00, 0, D_004B2060);
    }
}
