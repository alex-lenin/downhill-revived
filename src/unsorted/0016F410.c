/* UNIT 0x0016F410 0x0016F420 */
struct Obj {
    char pad0[0x204];
    int x204;
    char pad208[0x22C - 0x208];
    int x22C;
};

void func_0016F410(struct Obj *obj, int value)
{
    obj->x204 = value;
    obj->x22C = 0;
}
