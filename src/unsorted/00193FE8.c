/* UNIT 0x00193FE8 0x00194008 */
struct Obj {
    char pad0[0x12110];
    unsigned int x12110;
};

int func_00193FE8(struct Obj *obj, int bit)
{
    return ((1 << bit) & obj->x12110) != 0;
}
