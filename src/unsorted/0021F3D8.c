/* UNIT 0x0021F3D8 0x0021F3F8 .sdata=0x29DAE0:0x29DAE8 */
int D_0029DAE0 = 0;
int D_0029DAE4 = 1;

void func_0021F3D8(int enable)
{
    if (enable) {
        D_0029DAE4 = 0;
        D_0029DAE0 = 1;
    } else {
        D_0029DAE0 = 0;
        D_0029DAE4 = 1;
    }
}
