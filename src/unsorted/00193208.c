/* UNIT 0x00193208 0x001932B8 */
extern int D_0029E4CC;
extern int D_0076F9E8[];
extern int D_0076FA30[];
extern int D_00771508[][100];

void func_00193208(int index) {
    int i, tmp;

    if (++D_0076FA30[index] >= D_0076F9E8[index]) {
        D_0076FA30[index] = 0;
        for (i = D_0029E4CC + 1; i < D_0076F9E8[index]; i += 2) {
            tmp = D_00771508[index][i - 1];
            D_00771508[index][i - 1] = D_00771508[index][i];
            D_00771508[index][i] = tmp;
        }
        D_0029E4CC ^= 1;
    }
}
