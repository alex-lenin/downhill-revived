/* UNIT 0x00198E08 0x00198E80 */
extern int D_003F52F0[];
extern char D_0028DA80[];
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0018DC20(void *);

void func_00198E08(void *self)
{
    func_0020F518(0, 0xA0, 0x64, 0x32, 0xAA);
    func_0020FB80(0, 0x100, 0xC0, -0x5B, -0x20);
    func_0020F9F0(0, D_003F52F0[0], 0x100, 0xC8, D_0028DA80);
    func_0018DC20(self);
}
