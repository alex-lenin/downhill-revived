/* UNIT 0x00167B28 0x00167B88 */
struct Child {
    char pad0[4];
    char x4;
};

struct Global {
    char pad0[0x20];
    int x20;
    char pad24[0x110];
    struct Child *x134;
};

extern struct Global D_003F52D0;
extern int D_00398AE0[];
extern void func_00167BC0(void *obj, int a, int b, int c, int d, int e);
extern void func_0018DC20(void *obj);

void func_00167B28(void *obj) {
    struct Global *g = &D_003F52D0;

    if (g->x134) {
        g->x134->x4 = 0;
    }
    func_00167BC0(obj, g->x20, 20, 1, 170, D_00398AE0[0]);
    func_0018DC20(obj);
}
