/* UNIT 0x00175960 0x00175980 */
struct Inner {
    char pad0[0x8];
    char x8;
};

struct Mid {
    char pad0[0x20];
    struct Inner *x20;
};

struct Obj {
    char pad0[0xF78];
    struct Mid *xF78;
};

void func_00175960(struct Obj *obj)
{
    if (obj->xF78 != 0) {
        obj->xF78->x20->x8 = 1;
    }
}
