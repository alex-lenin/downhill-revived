/* UNIT 0x001E3A10 0x001E3A68 */
struct Child { char pad[4]; char x4; };
struct Global { char pad0[0x20]; int x20; char pad24[0x110]; struct Child *x134; };
extern struct Global D_003F52D0;
extern void func_001E3AA0(int, int, int, int, int, int);
extern void func_0018DC20(int);
void func_001E3A10(int arg0) {
    if (D_003F52D0.x134) D_003F52D0.x134->x4 = 0;
    func_001E3AA0(arg0, D_003F52D0.x20, 20, 1, 1, 170);
    func_0018DC20(arg0);
}
