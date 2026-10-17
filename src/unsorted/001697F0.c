/* UNIT 0x001697F0 0x00169808 */
struct Obj {
    char pad0[0x53C];
    int flags;
};

int func_001697F0(struct Obj *obj, int bit)
{
    int mask = 1 << bit;

    if (obj->flags & mask) {
        return 1;
    }
    return 0;
}
