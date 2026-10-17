/* UNIT 0x00238D10 0x00238D98 */
extern int D_0029E3D0;
extern int D_0029E3D8;
extern int D_0029E3DC;
extern int D_0029E3E0;
void func_00238D10(void) {
    int now, diff;
    now = D_0029E3D8;
    diff = now - D_0029E3DC;
    D_0029E3DC = now;
    D_0029E3D0 = diff;
    if (diff <= 0) D_0029E3D0 = 1;
    if (D_0029E3D0 >= 3) D_0029E3D0 = 2;
    D_0029E3D8 = now + 1;
    D_0029E3E0 += *(volatile int *)0x10000800;
    *(volatile int *)0x10000800 = 0;
}
