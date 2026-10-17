/* UNIT 0x0015F480 0x0015F4C8 */
struct Global { char pad0[0x118]; int x118; char pad11c[0x258]; char x374[0x180]; int x4f4; };
extern struct Global D_004B25A0;
extern void func_001C6EA8(void *);
void func_0015F480(void) {
    if (D_004B25A0.x118 == 2) {
        func_001C6EA8(D_004B25A0.x374);
        D_004B25A0.x4f4 = 1;
    }
}
