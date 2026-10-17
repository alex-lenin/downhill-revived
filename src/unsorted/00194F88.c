/* UNIT 0x00194F88 0x00194FA8 */
struct Obj {
    char pad0[0x428];
    signed char x428[1];
};

void func_00194F88(struct Obj *obj, int i, int value)
{
    if (value < obj->x428[i]) {
        obj->x428[i] = value;
    }
}
