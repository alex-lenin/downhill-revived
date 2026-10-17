/* UNIT 0x00210F20 0x00210F40 */
extern int D_0029DA50;

void func_00210F20(int value)
{
    if (value == 0) {
        D_0029DA50 = 0;
    } else if (D_0029DA50 == 0) {
        D_0029DA50 = value;
    }
}
