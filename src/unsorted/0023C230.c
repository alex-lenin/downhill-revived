/* UNIT 0x0023C230 0x0023C240 */
struct Obj {
    char pad0[0xA8];
    int xA8;
};

int func_0023C230(struct Obj *obj)
{
    obj->xA8 = 0;
    return 1;
}
