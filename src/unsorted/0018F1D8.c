/* UNIT 0x0018F1D8 0x0018F210 */
struct VEntry {
    short delta;
    short pad;
    void (*func)(void *, int, int, int);
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

void func_0018F1D8(struct Obj *obj, int a, int b, int c)
{
    struct Child *ch = obj->x500[obj->x534];
    ch->vtbl[4].func((char *)ch + ch->vtbl[4].delta, a, b, c);
}
