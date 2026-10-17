/* UNIT 0x001FB5B8 0x001FB5E8 .sbss=0x29E150:0x29E160 */
extern char D_00288730[];
extern char D_00288768[];
void *volatile D_0029E150 __attribute__((aligned(8), section(".sbss"))) = 0;
void *volatile D_0029E158 __attribute__((aligned(8), section(".sbss"))) = 0;
void func_001FB5B8(int enabled, int value) {
    if (value == 0xFFFF && enabled != 0) {
        D_0029E150 = D_00288768;
        D_0029E158 = D_00288730;
    }
}


