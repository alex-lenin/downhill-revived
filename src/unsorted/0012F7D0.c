/* UNIT 0x0012F7D0 0x0012F8D8 */
typedef struct Global {
    char pad0[0x10C];
    int x10C;
    char pad110[0x364 - 0x110];
    int x364;
} Global;

typedef struct Part {
    char pad0[0x36];
    unsigned short type : 5;
    unsigned short x36_5 : 11;
    char pad38[0x48];
} Part;

typedef struct Rider {
    long x0;
    char pad8[0x3FD0 - 8];
    char x3FD0[0x40C0 - 0x3FD0];
    char x40C0[0x43A0 - 0x40C0];
    Part x43A0;
    Part x4420;
    char pad44A0[0x4ECC - 0x44A0];
    void *x4ECC;
    char pad4ED0[0x6320 - 0x4ED0];
    char x6320[0x797C - 0x6320];
    int x797C;
    char pad7980[0x79AC - 0x7980];
    int x79AC;
} Rider;

extern Global D_004B25A0;
extern int func_00138560(void *p);
extern void func_00162D98(int type, int b);
extern void func_00155E78(void *p, int n);
extern void func_0012FE38(Rider *r, void *p);

static inline int GreaterThan(int count, int limit)
{
    return count > limit;
}

void func_0012F7D0(Rider *r)
{
    int a;
    int b;
    int ok;

    if ((unsigned char)func_00138560(r->x40C0)) {
        return;
    }
    b = r->x4420.type;
    a = r->x43A0.type;
    func_00162D98(b, r->x797C);
    if (a < 27 && b < 27) {
        return;
    }
    if (a == 27 || b == 27) {
        func_00155E78(r->x3FD0, 10);
        return;
    }
    if (a == 30 || b == 30) {
        ok = 0;
        if (D_004B25A0.x10C == 15) {
            ok = D_004B25A0.x364 == 1;
        }
        if (ok && GreaterThan(r->x79AC, 1)) {
            return;
        }
        if (r->x4ECC != r->x6320) {
            func_0012FE38(r, r->x6320);
        }
    }
}
