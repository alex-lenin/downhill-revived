/* UNIT 0x00117960 0x001179D0 */
struct Obj { char pad0[0x16]; unsigned char x16; char pad17; short x18; };
extern char D_0029EA20[];
extern int func_001177F8(void *, int);
extern int func_00118650(void *, struct Obj *, int, int);
int func_00117960(void *obj, int flag, struct Obj *arg) {
    int result = 0;
    if (arg) {
        if (flag) {
            if (arg->x16 == 1) result = func_001177F8(obj, arg->x18);
            else if (arg->x16 >= 2) result = func_00118650(D_0029EA20, arg, 0, 0);
        } else result = func_00118650(D_0029EA20, arg, 0, 0);
    }
    return result;
}
