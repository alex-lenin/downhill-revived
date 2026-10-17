/* UNIT 0x001E3568 0x001E35E0 */
typedef struct { char pad0[0x504]; int x504; int x508; } Obj;
typedef struct {
    char pad0[0x500];
    int x500;
    int x504[7];
    char x520[1][13];
} Global;
extern Global D_004C37A0;
extern void func_001E2F18(Global *, int, int);
void func_001E3568(Obj *p, int value) {
    int mode = p->x508;
    if (mode == 0) {
        func_001E2F18(&D_004C37A0, p->x504, value);
    } else if (mode == 1) {
        D_004C37A0.x520[D_004C37A0.x500][p->x504] = value;
        D_004C37A0.x504[D_004C37A0.x500] = mode;
    }
}
