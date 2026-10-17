/* UNIT 0x001F95D0 0x001F9610 */
extern char D_003F52D0[];
extern void func_001B5370(void *, int);
extern void func_001B5AF0(void *, int, int);
void func_001F95D0(void *unused, int value) {
    func_001B5370(D_003F52D0, value);
    func_001B5AF0(D_003F52D0, 3, 1);
}
