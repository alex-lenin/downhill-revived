/* UNIT 0x00190410 0x00190448 */
struct Obj {
    char pad0[0x12C];
    char x12C;
    char x12D;
    char pad12E[0x18C - 0x12E];
    int x18C;
};

extern void func_001908D0(struct Obj *, int);

void func_00190410(struct Obj *obj, int value)
{
    obj->x12C = value;
    if (obj->x18C != 0) {
        func_001908D0(obj, value);
    } else {
        obj->x12D = 1;
    }
}
