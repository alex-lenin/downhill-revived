/* UNIT 0x0021F620 0x0021F788 */
extern int D_0029E2B0;
extern unsigned char D_0029E2A8[8];
extern int D_00660D00[8];
extern int D_00660DA0[8];
extern int D_00660DC0[8];
extern unsigned char D_00660DE0[8][6];
extern unsigned char D_00660E10[8][6];
extern int func_00245DA0(int);
extern void func_0021F880(int);

void func_0021F620(void) {
    int i;
    int j;
    int tmp;
    tmp = func_00245DA0(0);
    D_0029E2B0 = tmp;
    if (tmp != 0) {
        D_0029E2B0 = 1;
    }
    for (i = 0; i < 8; i++) {
        D_00660D00[i] = 0;
        D_0029E2A8[i] = 0;
        if (D_0029E2B0 == 1) {
            D_00660DA0[i] = i * 2;
        } else if (D_0029E2B0 == 2) {
            D_00660DA0[i] = i * 2 + 1;
        } else {
            D_00660DA0[i] = i;
        }
        func_0021F880(i);
        D_00660DC0[i] = 0;
        for (j = 0; j < 6; j++) {
            D_00660DE0[i][j] = 0;
            D_00660E10[i][j] = 0;
        }
    }
}
