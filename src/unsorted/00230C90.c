/* UNIT 0x00230C90 0x00230CA0 */
struct Obj {
    char pad0[0x8090];
    int x8090;
};

int func_00230C90(struct Obj *obj)
{
    return obj->x8090;
}
