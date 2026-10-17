/* UNIT 0x001B5B70 0x001B5B88 */
struct Obj {
    char pad0[0x58];
    int *x58;
};

void func_001B5B70(struct Obj *obj, int value)
{
    if (obj->x58 != 0) {
        *obj->x58 = value;
    }
}
