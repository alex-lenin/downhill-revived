/* UNIT 0x002082E8 0x00208318 */
extern char D_004D02C0[];
extern void func_00207F80(void *);
void func_002082E8(int enabled, int id) {
    if (id == 0xFFFF) {
        if (enabled != 0) {
            func_00207F80(D_004D02C0);
        }
    }
}
