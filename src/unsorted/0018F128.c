/* UNIT 0x0018F128 0x0018F160 */
struct VEntry {
    short delta;
    short pad;
    void (*func)(void *);
};

struct Child {
    char pad0[0x4F8];
    struct VEntry *vtbl;
};

struct Obj {
    char pad0[0x500];
    struct Child *x500[13];
    int x534;
};

void func_0018F128(struct Obj *obj)
{
    struct Child *c = obj->x500[obj->x534];
    c->vtbl[3].func((char *)c + c->vtbl[3].delta);
}
