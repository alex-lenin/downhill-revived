/* UNIT 0x002350D0 0x002350E8 */
struct Obj {
    char pad0[0x5F8];
    unsigned char x5F8;
};

void func_002350D0(struct Obj *obj)
{
    if (obj->x5F8 == 0) {
        obj->x5F8 = 1;
    }
}
