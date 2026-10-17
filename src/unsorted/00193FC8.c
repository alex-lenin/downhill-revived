/* UNIT 0x00193FC8 0x00193FE8 */
struct Obj {
    char pad0[0x12180];
    unsigned int x12180;
};

int func_00193FC8(struct Obj *obj, int bit)
{
    return ((1 << bit) & obj->x12180) != 0;
}
