/* UNIT 0x0023C278 0x0023C288 */
struct Obj {
    char pad0[0xA8];
    int xA8;
};

int func_0023C278(struct Obj *obj)
{
    obj->xA8 = 1;
    return 1;
}
