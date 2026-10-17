/* UNIT 0x0021F820 0x0021F880 */
extern int D_0029E2B8;
extern int func_00246030(int);
extern void func_0021F620(void);
void func_0021F820(void) {
    int *p;
    int i;
    for (i = 0, p = &D_0029E2B8; i < 2; i++, p++) {
        int value = func_00246030(i);
        if (*p != value) {
            *p = value;
            func_0021F620();
        }
    }
}
