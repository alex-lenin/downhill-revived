/* UNIT 0x001FA4B0 0x001FA590 */
typedef struct Selection {
    int x0;
    int x4;
    int x8;
} Selection;

extern char D_004C84E8[];
extern char D_0029D980[];
extern char D_00296FE8[];
extern char D_00276090[][9];
extern char D_00275FA8[][9];
extern char D_00275F40[][9];
extern int func_00254D68(char *, const char *, ...);

char *func_001FA4B0(Selection *sel, int kind)
{
    switch (kind) {
    case 0:
        func_00254D68(D_004C84E8, D_0029D980, D_00296FE8);
        break;
    case 1:
        func_00254D68(D_004C84E8, D_0029D980, D_00276090[sel->x0]);
        break;
    case 2:
        func_00254D68(D_004C84E8, D_0029D980, D_00275FA8[sel->x4]);
        break;
    case 3:
        func_00254D68(D_004C84E8, D_0029D980, D_00275F40[sel->x8]);
        break;
    }
    return D_004C84E8;
}
