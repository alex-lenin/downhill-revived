/* UNIT 0x001E2A40 0x001E2D78 */
typedef struct Obj {
    char pad0[0x500];
    int x500;
    char pad504[0x520 - 0x504];
    unsigned char x520[4][13];
    unsigned char x554[4];
} Obj;

typedef struct Menu {
    char pad0[0x20];
    int x20;
} Menu;

typedef struct Global {
    char pad0[0x288];
    int x288;
    char pad28C[4];
    int x290;
} Global;

extern Menu D_003F52D0;
extern Global D_004B25A0;
extern char D_004225A0[];
extern char D_00293A70[];
extern char D_00293A88[];
extern char D_00293A98[];
extern char D_00293AA8[];
extern char D_0029D718[];
extern char *D_0026EFE8[];

extern void func_0020F518(int a, int b, int c, int d, int e);
extern void func_0020FB80(int a, int b, int c, int d, int e);
extern void func_0020F9A8(int a, int b, int c, int d, char *e);
extern int func_00254D68(char *dst, char *fmt, ...);
extern int func_00194C80(void *p, int id);
extern int func_00194A10(void *p, int id);

void func_001E2A40(Obj *obj)
{
    char buf[48];
    unsigned char *row;
    int y;
    int i;
    int n;

    if (obj->x500 >= 4) {
        return;
    }
    y = 0x2F;
    i = 0;
    row = &obj->x520[obj->x500][0];
    func_0020F518(0, 0x58, 0x98, 0x49, 0xAA);
    func_0020FB80(0, 0x140, 0xE0, -0x6B, -0x20);
    func_00254D68(buf, D_00293A70, obj->x554[obj->x500]);
    func_0020F9A8(0, D_003F52D0.x20, 0xDC, 0x14, buf);
    func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
    func_0020F9A8(0, D_003F52D0.x20, 0x96, 0x23, D_00293A88);
    func_0020F9A8(0, D_003F52D0.x20, 0x140, 0x23, D_00293A98);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    if (row[0] != '!') {
        while (i < 12) {
            n = i + 1;
            func_00254D68(buf, D_0029D718, n);
            func_0020F9A8(0, D_003F52D0.x20, 0xD2, y, buf);
            func_0020F9A8(0, D_003F52D0.x20, 0x140, y, D_0026EFE8[row[i]]);
            y += 10;
            i = n;
            if (row[n] == '!') {
                break;
            }
        }
    }
    if (D_004B25A0.x288 == 1 && obj->x554[obj->x500] != 0) {
        func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
        func_00254D68(buf, D_00293AA8, func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, obj->x500 + 6), obj->x554[obj->x500]);
        func_0020F9A8(0, 0, 0xEC, y, buf);
    } else if (D_004B25A0.x288 == 2 && obj->x554[obj->x500] != 0) {
        func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
        func_00254D68(buf, D_00293AA8, func_00194A10(D_004225A0, obj->x500 + 6), obj->x554[obj->x500]);
        func_0020F9A8(0, 0, 0xEC, y, buf);
    }
}
