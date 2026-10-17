/* UNIT 0x0015FC70 0x0015FCB8 */
struct Child { char pad0[4]; char x4; };
struct GlobalA { char pad0[0x134]; struct Child *x134; };
struct GlobalB { char pad0[0x5be4]; int x5be4; int x5be8; };
extern struct GlobalA D_003F52D0;
extern struct GlobalB D_00398C40;
extern char D_0043DD50[];
extern void func_00190660(struct GlobalA *, void *);
void func_0015FC70(void) {
    D_00398C40.x5be4 = 1;
    D_00398C40.x5be8 = 1;
    if (D_003F52D0.x134) D_003F52D0.x134->x4 = 0;
    func_00190660(&D_003F52D0, D_0043DD50);
}
