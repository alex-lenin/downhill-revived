/* UNIT 0x0021F788 0x0021F818 */
extern int D_00660C80[2][4];
extern int D_00660CE0[];
extern int D_00660D00[];
extern int D_0029E2B8[2];
extern unsigned char D_0029E2A8[8];

void func_0021F788(void)
{
    int i, j, id;

    for (i = 0; i < 2; i++) {
        D_0029E2B8[i] = 0;
        for (j = 0; j < 4; j++) {
            id = D_00660C80[i][j];
            D_00660CE0[id] = 0;
            D_00660D00[id] = 0;
            D_0029E2A8[id] = 0;
        }
    }
}
