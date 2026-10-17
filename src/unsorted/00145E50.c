/* UNIT 0x00145E50 0x00145E80 */
struct Obj {
    int x0;
    int x4;
};
extern void func_001450A0(struct Obj *, int);
void func_00145E50(struct Obj *arg0, int arg1) {
    if (arg1 != 0) {
        if (arg0->x4 != 4) {
            func_001450A0(arg0, 4);
        }
    }
}
