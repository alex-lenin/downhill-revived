/* UNIT 0x001764F8 0x00176508 */
struct Obj {
    char pad0[0xFA4];
    int xFA4;
    int xFA8;
    int xFAC;
};

void func_001764F8(struct Obj *obj)
{
    obj->xFA4 = 0;
    obj->xFA8 = 0;
    obj->xFAC = 0;
}
