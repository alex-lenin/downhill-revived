/* UNIT 0x001B20B0 0x001B20F8 */
extern void func_00230708(void *obj, int n);
extern char D_00287EE8[];

struct Obj {
    char pad0[0x8098];
    void *x8098;
    char pad809C[0x80C0 - 0x809C];
    int x80C0;
};

struct Obj *func_001B20B0(struct Obj *obj)
{
    func_00230708(obj, 9);
    obj->x8098 = D_00287EE8;
    obj->x80C0 = 0;
    return obj;
}
