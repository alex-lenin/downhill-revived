/* UNIT 0x001425B0 0x001425E0 */
struct Obj {
    char pad0[0x58];
    int x58;
};
extern int func_00222B08(int, int);
void func_001425B0(struct Obj *arg0, int arg1, int arg2) {
    arg0->x58 = func_00222B08(arg1, arg2);
}
