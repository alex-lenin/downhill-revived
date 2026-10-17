/* UNIT 0x001A2280 0x001A22D8 */
struct A {
 char pad[0x144];
 int x144, x148, x14c, x150, x154, x158, x15c, x160;
};
extern int D_004225A0[];
void func_001A2280(struct A *arg0) {
 arg0->x148 = 1;
 arg0->x154 = 4;
 arg0->x144 = 0;
 arg0->x14c = 0;
 arg0->x150 = 0;
 arg0->x15c = 0;

 arg0->x158 = D_004225A0[0x1220c/4] / 20;
 arg0->x160 = 9;
}

