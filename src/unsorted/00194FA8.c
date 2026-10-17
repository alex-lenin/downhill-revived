/* UNIT 0x00194FA8 0x00194FD8 */
struct Obj {
    char pad0[0x320];
    int x320[0x4A];
    char pad448[1];
    signed char x449[1];
};

void func_00194FA8(struct Obj *obj, int i, int value, int arg3)
{
    if (value < obj->x320[i]) {
        obj->x320[i] = value;
        obj->x449[i] = arg3;
    }
}
