/* UNIT 0x001962D0 0x001962F0 */
struct Obj {
    int x0[6];
};

void func_001962D0(struct Obj *obj, unsigned int index, int value)
{
    if (index < 6) {
        obj->x0[index] = value;
    }
}
