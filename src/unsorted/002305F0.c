/* UNIT 0x002305F0 0x00230610 */
struct Obj {
    char pad0[0x50000];
    int x50000;
    int x50004;
    int x50008;
};

int func_002305F0(struct Obj *obj, int value)
{
    obj->x50004 = 0;
    obj->x50000 = 0;
    obj->x50008 = value;
    return 1;
}
