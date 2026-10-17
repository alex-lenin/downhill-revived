/* UNIT 0x0023B880 0x0023BBB8 */
typedef struct Obj {
    unsigned int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    char pad14[8];
    unsigned int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    unsigned int x38;
    int x3C;
    int x40;
    int x44;
} Obj;

extern int func_0023B0F8(Obj *, unsigned int);
extern void func_0023B140(int);
extern void func_0023B1B0(int);
extern int func_0025A700(int);
extern int func_0025A720(int);

int func_0023B880(Obj *o) {
    unsigned int cmd = o->x38;
    unsigned int hi = (cmd >> 8) & 0xF;
    unsigned int n = ((cmd >> 0x10) & 3) + hi;
    unsigned int s2 = o->x1C - n * 0x10;
    unsigned int s5 = o->x24 + n;
    int s3 = o->x28 | 0x100;
    int s4 = o->x20;
    unsigned int s6 = cmd & 0x7F;
    int s1;
    int t6;
    int flag;
    int n8;
    int sz;
    int t4;
    unsigned int t0;
    int t5;

    func_0025A720(o->x40);
    if (s2 < o->x0) {
        n8 = o->x8;
        sz = n8 << 11;
        s5 = (o->x0 - s2) >> 4;
        s4 = o->x4 & 0x0FFFFFFF;
        s2 += sz;
        flag = (o->x1C != o->x0 && o->x1C != o->x0 + sz) ? 3 : 0;
        s3 = ((o->x28 & 0x0FFFFFFF) | (flag << 28)) | 0x100;
        if ((n8 - o->xC) % n8 < 0 || (n8 - o->xC) % n8 >= o->x10) {
            o->xC = n8 - 1;
            o->x10 = o->x10 + 1;
        }
    } else {
        s1 = func_0023B0F8(o, o->x1C);
        t6 = func_0023B0F8(o, s2);
        if (s1 != t6) {
            unsigned int rem1;
            int rem2;
            int rd;
            t4 = o->x8;
            t0 = o->x0;
            rem1 = (o->x1C - t0) % (t4 << 11);
            rd = t6 + t4 - o->xC;
            t5 = o->x10;
            s5 = (t0 + (s1 << 11) - s2) >> 4;
            s4 = (s1 * 0x10 + o->x4) & 0x0FFFFFFF;
            rem2 = (o->xC + t5) % t4;
            flag = ((t0 + rem1) != (t0 + (rem2 << 11))) ? 3 : 0;
            s3 = ((o->x28 & 0x0FFFFFFF) | (flag << 28)) | 0x100;
            if (rd % t4 < 0 || rd % t4 >= t5) {
                o->xC = t6;
                o->x10 = t5 + 1;
            }
        }
    }
    if (o->x2C != 0 && o->x30 != 0) {
        *(volatile int *)0x1000B010 = o->x2C;
        *(volatile int *)0x1000B020 = o->x30;
        func_0023B140(o->x34 | 0x100);
    }
    if (o->x10 != 0) {
        if (*(volatile int *)0x10002010 < 0) {
            while (*(volatile int *)0x10002010 < 0) {
            }
        }
        *(volatile int *)0x10002000 = s6;
        if (*(volatile int *)0x10002010 < 0) {
            while (*(volatile int *)0x10002010 < 0) {
            }
        }
    }
    *(volatile unsigned int *)0x1000B410 = s2;
    *(volatile int *)0x1000B430 = s4;
    *(volatile unsigned int *)0x1000B420 = s5;
    if (o->x10 != 0) {
        func_0023B1B0(s3);
    }
    *(volatile int *)0x10002010 = o->x3C;
    o->x44 = 1;
    func_0025A700(o->x40);
    return 1;
}
