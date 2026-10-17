/* UNIT 0x00139B90 0x00139BC0 */
extern char D_0032FC60[];
extern void func_00138900(void *);
void func_00139B90(int enabled, int kind) {
    if (kind == 0xFFFF) {
        if (enabled != 0) {
            func_00138900(D_0032FC60);
        }
    }
}
