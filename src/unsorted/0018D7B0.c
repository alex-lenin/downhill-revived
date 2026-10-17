/* UNIT 0x0018D7B0 0x0018D7D0 */
struct Obj {
    char pad0[0x8];
    int *x8;
    unsigned char xC;
};

void func_0018D7B0(struct Obj *obj)
{
    if (obj->x8 != 0) {
        *obj->x8 = 0;
        obj->x8 = 0;
    }
    obj->xC = 0;
}
