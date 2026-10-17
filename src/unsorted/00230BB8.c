/* UNIT 0x00230BB8 0x00230BD0 */
struct Obj {
    char pad0[0x808C];
    int x808C;
    int x8090;
};

void func_00230BB8(struct Obj *obj)
{
    obj->x808C = 0;
    obj->x8090 = 0;
}
