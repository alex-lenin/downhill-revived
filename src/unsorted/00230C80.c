/* UNIT 0x00230C80 0x00230C90 */
struct Obj {
    char pad0[0x808C];
    int x808C;
};

int func_00230C80(struct Obj *obj)
{
    return obj->x808C;
}
