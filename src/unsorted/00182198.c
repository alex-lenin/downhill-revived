/* UNIT 0x00182198 0x001821E8 */
extern int D_0029E4B8;
extern int D_0076F9B0[];
void func_00182198(int value)
{
    if (D_0029E4B8 < 12) {
        D_0076F9B0[D_0029E4B8++] = value;
        D_0076F9B0[D_0029E4B8] = 33;
    }
}
