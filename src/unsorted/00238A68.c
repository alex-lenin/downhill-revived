/* UNIT 0x00238A68 0x00238B20 */
extern void func_00238F68(int);                               /* extern */
extern int D_0029E3D0;
extern int D_0029E3D4;
extern int D_0029E3D8;
extern int D_0029E3DC;
extern int D_0029E3E0;
extern int D_0029E3E4;
extern int D_0029E3E8;
extern int D_0029E3EC;
extern int D_0029E3F0;
extern int D_0029E3F8;
extern float D_0029E3FC;
extern float D_0029E400;

void func_00238A68(int arg0) {
    float temp_f1;

    temp_f1 = (float) arg0;
    D_0029E3D0 = 1;
    D_0029E3F0 = 0x3C;
    D_0029E3D4 = arg0;
    D_0029E3FC = temp_f1;
    D_0029E3E0 = 0;
    D_0029E3E4 = 0;
    D_0029E400 = 1.0f / temp_f1;
    D_0029E3E8 = 0;
    D_0029E3EC = 0;
    D_0029E3D8 = 0;
    D_0029E3DC = 0;
    D_0029E3F8 = 0;
    func_00238F68(arg0);
    *(volatile int *)0x10000810 = 0x82;
    *(volatile int *)0x10000800 = 0;
}
