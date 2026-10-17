/* UNIT 0x00235138 0x00235150 */
struct Obj {
    char pad0[0x5F8];
    unsigned char x5F8;
};

void func_00235138(struct Obj *obj)
{
    if (obj->x5F8 != 0) {
        obj->x5F8 = 0;
    }
}
