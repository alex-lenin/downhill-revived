/* UNIT 0x001B5AB0 0x001B5AC8 */
extern int D_0029E10C;

struct Obj {
    char pad0[0x40];
    int x40;
};

void func_001B5AB0(struct Obj *obj, int offset)
{
    obj->x40 = D_0029E10C + offset;
}
