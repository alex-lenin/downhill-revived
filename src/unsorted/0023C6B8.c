/* UNIT 0x0023C6B8 0x0023C6E0 */
struct Arg { int x0; int x4; };
extern char D_002993F0[];
extern void func_00254D08(void *, int);
int func_0023C6B8(void *unused, struct Arg *arg) {
    func_00254D08(D_002993F0, arg->x4);
    return 1;
}
