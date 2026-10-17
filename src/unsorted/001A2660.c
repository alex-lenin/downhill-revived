/* UNIT 0x001A2660 0x001A2C58 .sdata=0x29D2F8:0x29D308 */
typedef struct Bike { char pad0[0x796C]; int x796C; } Bike;
typedef struct Obj {
    char pad0[0xC8];
    Bike *xC8;
    int xCC;
    int xD0;
    int xD4;
    int xD8;
    int xDC;
    Bike *xE0;
    int xE4;
    int xE8;
    int xEC;
    int xF0;
    int xF4;
    char padF8[0x148 - 0xF8];
    int x148;
    int x14C;
} Obj;

typedef struct Fonts { char pad0[0x20]; int x20; } Fonts;

extern Fonts D_003F52D0;
extern char D_004C8508[];
extern int D_004B2828[];
extern char D_0028E790[];
extern char D_0028E7A0[];
extern char D_0028E7B0[];
extern char D_0029D2E0[];
extern char D_0029D2E8[];
extern char D_0029D2F0[];
char D_0029D2F8[8] = "%d";
extern char D_0029D308[];
extern char D_0029D310[];
extern int func_001FA168(void *, int, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00254D68(char *, char *, ...);

int func_001A2660(Obj *obj)
{
    char buf[48];
    int r30;
    int r34;
    int r38;
    int y;
    Fonts *f;
    int cnt1;
    int div1;
    int sum1;
    int sum2;
    int div2;
    int alpha;

    r30 = 0;
    r38 = 0x50;
    sum1 = func_001FA168(D_004C8508, obj->xDC, (obj->xC8->x796C >> 5) - 1);
    sum2 = obj->xCC;
    r34 = obj->xD4;
    r38 = (D_004B2828[0] >= 2) ? 0x58 : r38;
    cnt1 = obj->xD8;
    if (D_004B2828[0] == 2) {
        sum1 += func_001FA168(D_004C8508, obj->xF4, (obj->xE0->x796C >> 5) - 1);
        sum2 += obj->xE4;
        r34 += obj->xEC;
        cnt1 += obj->xF0;
    }
    alpha = (obj->x148 == 1) ? obj->x14C : 0xAA;
    func_0020F518(0, 0xDD, 0xDB, 0xA, alpha);
    func_0020FB80(0, 0x160, 0x100, -0x72, -0x20);
    f = &D_003F52D0;
    func_0020F9F0(0, f->x20, 0x100, r38 | 2, D_0028E790);
    alpha = (obj->x148 == 1) ? obj->x14C : 0xAA;
    y = r38 + 0xF;
    func_0020F518(0, 0x6E, 0x6E, 0xC8, alpha);
    func_0020FB80(0, 0x100, 0xC0, -0x52, -0x20);
    func_0020F9A8(0, f->x20, 0x74, y, D_0028E7A0);
    func_0020F9A8(0, f->x20, 0xE8, y, D_0029D2E0);
    func_0020F9A8(0, f->x20, 0x164, y, D_0029D2E8);
    if (obj->x148 >= 2) {
        func_0020FB80(0, 0x100, 0xC0, -0x52, -0x20);
        alpha = (obj->x148 == 1) ? obj->x14C : 0xAA;
        y = r38 + 0x1A;
        func_0020F518(0, 0x50, 0x78, 0x50, alpha);
        func_0020F9A8(0, f->x20, 0x74, y, D_0029D2F0);
        func_00254D68(buf, D_0029D2F8, sum1);
        func_0020F9A8(0, f->x20, 0xE8, y, buf);
        func_00254D68(buf, "$%d", sum1 / 20);
        func_0020F9A8(0, f->x20, 0x164, y, buf);
        if (obj->x148 >= 3) {
            r30 = sum1 / 20;
            alpha = (obj->x148 == 3) ? obj->x14C : 0xAA;
            y = r38 + 0x24;
            func_0020F518(0, 0x50, 0x78, 0x50, alpha);
            func_0020F9A8(0, f->x20, 0x74, y, D_0029D308);
            func_00254D68(buf, D_0029D2F8, sum2);
            func_0020F9A8(0, f->x20, 0xE8, y, buf);
            func_00254D68(buf, "$%d", sum2 / 20);
            func_0020F9A8(0, f->x20, 0x164, y, buf);
            if (obj->x148 >= 4) {
                r30 = sum1 / 20 + sum2 / 20;
                y = r38 + 0x2E;
                func_0020F9A8(0, f->x20, 0x74, y, D_0029D310);
                alpha = (obj->x148 == 4) ? obj->x14C : 0xAA;
                func_0020F518(0, 0x50, 0x78, 0x50, alpha);
                func_00254D68(buf, D_0029D2F8, r34);
                func_0020F9A8(0, f->x20, 0xE8, y, buf);
                func_00254D68(buf, "$%d", r34 / 20);
                func_0020F9A8(0, f->x20, 0x164, y, buf);
                if (cnt1 > 0) {
                    if (obj->x148 >= 5) {
                        y = r38 + 0x38;
                        r30 = sum1 / 20 + sum2 / 20 + r34 / 20;
                        func_0020F9A8(0, f->x20, 0x74, y, D_0028E7B0);
                        alpha = (obj->x148 == 5) ? obj->x14C : 0xAA;
                        func_0020F518(0, 0x50, 0x78, 0x50, alpha);
                        func_00254D68(buf, D_0029D2F8, cnt1);
                        func_0020F9A8(0, f->x20, 0xE8, y, buf);
                        func_00254D68(buf, "$%d", cnt1 / 20);
                        func_0020F9A8(0, f->x20, 0x164, y, buf);
                    }
                    if (obj->x148 >= 6) {
                        r30 = sum1 / 20 + sum2 / 20 + r34 / 20 + cnt1 / 20;
                    }
                } else if (obj->x148 >= 5) {
                    r30 = sum1 / 20 + sum2 / 20 + r34 / 20;
                }
            }
        }
    }
    return r30;
}
