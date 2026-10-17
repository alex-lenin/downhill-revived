/* UNIT 0x001DBAB0 0x001DBAE0 */
extern char D_004BD840[];
extern void func_00208AB0(void *);
void func_001DBAB0(int enabled, int code) {
    if (code == 0xFFFF) {
        if (enabled != 0) {
            func_00208AB0(D_004BD840);
        }
    }
}
