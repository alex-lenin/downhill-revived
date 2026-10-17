/* UNIT 0x0020F6C0 0x0020F7D0 */
extern int D_0029E188;
extern int D_0029E18C;

int func_0020F6C0(unsigned char c, int *width)
{
    *width = 0;
    if (D_0029E188 == 1 && D_0029E18C != 0) {
        if (c == 'c' || c == 'C' || c == 'm' || c == 'M' || c == 'n' || c == 'N' ||
            c == 'o' || c == 'O' || c == 'w' || c == 'W' || c == 'u' || c == 'U' ||
            c == 'h' || c == 'H' || c == 'g' || c == 'G' || c == 'b' || c == 'B' ||
            c == '8') {
            *width = 10;
            return 10;
        }
        if (c == 'i' || c == 'I' || c == '1' || c == 'l' || c == ':' || c == '.') {
            return -16;
        }
    }
    return 0;
}
