/* UNIT 0x0013FE50 0x0013FEA8 */
struct Local { int x0, x4, x8; short xC; short xE; };
extern void func_00203590(struct Local *, int, int, int);
extern void func_00203A80(struct Local *, int, float);
void func_0013FE50(int *arg0) {
    struct Local local;
    local.x8 = 0;
    func_00203590(&local, 0x26, 0, 1);
    func_00203A80(&local, 1, (float)(*arg0 * 2));
}
