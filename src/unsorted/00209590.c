/* UNIT 0x00209590 0x002095D8 */
struct List { int x0; struct List *x4; struct List *x8; int xC; };
extern struct List D_004D0318, D_004D0328, D_004D0338;
void func_00209590(int arg0, int arg1) {
    if (arg1 == 0xFFFF && arg0) {
        D_004D0318.x8 = &D_004D0318;
        D_004D0328.x8 = &D_004D0328;
        D_004D0338.x8 = &D_004D0338;
        D_004D0318.x4 = &D_004D0318;
        D_004D0328.x4 = &D_004D0328;
        D_004D0338.x4 = &D_004D0338;
    }
}
