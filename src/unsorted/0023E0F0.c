/* UNIT 0x0023E0F0 0x0023E100 */
struct Obj {
    char pad0[0xC];
    int xC;
};

int func_0023E0F0(struct Obj *obj)
{
    return obj->xC == 0;
}
