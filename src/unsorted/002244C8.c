/* UNIT 0x002244C8 0x002244F0 .sbss=0x29E2C4:0x29E2CC */
int D_0029E2C4 __attribute__((section(".sbss"))) = 0;
int D_0029E2C8 __attribute__((section(".sbss"))) = 0;
extern int D_006754B8[];
void func_002244C8(int active, int value) {
    if (value == 0xFFFF && active != 0) {
        D_0029E2C4 = D_006754B8[0];
        D_0029E2C8 = D_006754B8[0];
    }
}
