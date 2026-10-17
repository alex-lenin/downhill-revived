/* UNIT 0x0015BFA0 0x0015BFC0 */
struct Obj {
    struct Obj *x0;
    int x4;
    char pad8[0x4];
    int xC;
    char pad10[0x8];
    int x18;
    char pad1C[0x8];
    int x24;
    char pad28[0x8];
    int x30;
};

void func_0015BFA0(struct Obj *obj, int value)
{
    obj->x0 = (struct Obj *)&obj->x4;
    obj->x4 = value;
    obj->xC = value;
    obj->x18 = value;
    obj->x24 = value;
    obj->x30 = value;
}
