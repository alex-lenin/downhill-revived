/* UNIT 0x00220018 0x00220130 */
extern unsigned char D_0029E2A0[8];
extern int D_00660E40[][2];
extern unsigned char D_00660E10[][6];

void func_00220018(int i, unsigned char a, unsigned char b, int value)
{
    if (D_0029E2A0[i] == 0) {
        return;
    }
    if (value == 0) {
        if (a == 0) {
            D_00660E10[i][0] = 0;
            D_00660E40[i][0] = 0;
        }
        if (b == 0) {
            D_00660E10[i][1] = 0;
            D_00660E40[i][1] = 0;
        }
    } else {
        if (a != 0) {
            D_00660E10[i][0] = a;
            D_00660E40[i][0] = value;
        }
        if (b != 0 && D_00660E40[i][1] < value && D_00660E10[i][1] < b) {
            D_00660E10[i][1] = b;
            D_00660E40[i][1] = value;
        }
    }
}
