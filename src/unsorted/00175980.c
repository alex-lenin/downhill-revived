/* UNIT 0x00175980 0x00175998 */
struct Sub {
    char pad0[4];
    char x4;
};

struct Obj {
    char pad0[0x1180];
    struct Sub *x1180;
};

void func_00175980(struct Obj *obj)
{
    if (obj->x1180 != 0) {
        obj->x1180->x4 = 0;
    }
}
