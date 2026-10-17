/* UNIT 0x0023C290 0x0023C2A0 */
struct Obj {
    char pad0[0xA8];
    int xA8;
};

int func_0023C290(struct Obj *obj, int value)
{
    int old = obj->xA8;
    obj->xA8 = value;
    return old;
}
