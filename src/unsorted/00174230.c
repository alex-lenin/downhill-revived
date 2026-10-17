/* UNIT 0x00174230 0x00174270 */
struct Arg {
    char pad0[0xC];
    char *xC;
};

struct Obj {
    char pad0[0x7A5C];
    int x7A5C;
};

extern void func_0012FE98(struct Obj *obj, char *p, int arg2);

void func_00174230(struct Arg *arg, struct Obj *obj, int arg2)
{
    if (obj->x7A5C == 1 || obj->x7A5C == 3) {
        func_0012FE98(obj, arg->xC + 0x10, arg2);
    }
}
