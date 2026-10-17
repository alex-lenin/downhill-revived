/* UNIT 0x00238F68 0x00239028 */
extern int D_0029E3F0;
extern int D_0029E3F4;

void func_00238F68(int value)
{
    int max = D_0029E3F0;

    if (value >= max) {
        D_0029E3F4 = max;
    } else if (value >= max / 2) {
        D_0029E3F4 = max / 2;
    } else if (value >= max / 3) {
        D_0029E3F4 = max / 3;
    } else if (value >= max / 4) {
        D_0029E3F4 = max / 4;
    } else if (value >= max / 5) {
        D_0029E3F4 = max / 5;
    } else {
        D_0029E3F4 = max / 6;
    }
}
