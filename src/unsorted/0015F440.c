/* UNIT 0x0015F440 0x0015F480 */
struct Obj { char pad0[0x118]; int x118; char pad11C[0x3D8]; int x4F4; };
extern struct Obj D_004B25A0;
extern void func_001C6E88(void *);
void func_0015F440(void) {
    if (D_004B25A0.x118 == 2) {
        func_001C6E88((char *)&D_004B25A0 + 0x374);
        D_004B25A0.x4F4 = 0;
    }
}
