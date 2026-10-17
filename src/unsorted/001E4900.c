/* UNIT 0x001E4900 0x001E4950 */
struct Obj { char pad0[0x500]; int x500; };
extern char D_004B28FC[];
extern char D_003F53FD[];
extern struct Obj D_004C5640;
extern void func_0015C990(void);
extern void func_001E4120(struct Obj *);
void func_001E4900(void *obj, int value) {
    D_004B28FC[0] = 1;
    func_0015C990();
    D_004C5640.x500 = value;
    func_001E4120(&D_004C5640);
    D_003F53FD[0] = 1;
}
