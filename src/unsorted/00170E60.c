/* UNIT 0x00170E60 0x00170E70 */
struct Obj {
    char pad0[0x8];
    int x8;
    int xC;
};

void func_00170E60(struct Obj *obj)
{
    obj->x8 = 0;
    obj->xC = 0;
}
