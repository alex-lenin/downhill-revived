/* UNIT 0x0012AD60 0x0012AD80 */
struct Obj {
    char pad0[0x180];
    int x180;
};

void func_0012AD60(struct Obj *obj)
{
    obj->x180--;
    if (obj->x180 <= 0) {
        obj->x180 = 0;
    }
}
