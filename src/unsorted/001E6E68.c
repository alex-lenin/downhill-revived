/* UNIT 0x001E6E68 0x001E6E80 */
struct Obj {
    char pad0[0x10F0];
    int x10F0;
};

void func_001E6E68(struct Obj *obj, int enable)
{
    if (enable) {
        obj->x10F0 = 1;
    } else {
        obj->x10F0 = 0;
    }
}
