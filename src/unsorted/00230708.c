/* UNIT 0x00230708 0x00230728 */
extern int D_002887A0[];

struct Obj {
    char pad0[0x8088];
    int x8088;
    char pad808C[0x8098 - 0x808C];
    int *x8098; /* vtable D_002887A0 */
};

struct Obj *func_00230708(struct Obj *obj, int value)
{
    obj->x8098 = D_002887A0;
    obj->x8088 = value;
    return obj;
}
