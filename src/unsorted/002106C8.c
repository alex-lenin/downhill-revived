/* UNIT 0x002106C8 0x002106D8 */
struct Obj {
    char pad0[0x30];
    int x30;
    int x34;
};

void func_002106C8(struct Obj *obj)
{
    obj->x30 = 0;
    obj->x34 = 0;
}
