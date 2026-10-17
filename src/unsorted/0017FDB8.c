/* UNIT 0x0017FDB8 0x0017FDF8 */
struct Packet {
    unsigned int x0_0 : 7;
    unsigned int slot : 11;
    unsigned int x0_18 : 14;
};

struct Ref {
    struct Packet *x0;
};

struct Obj {
    char pad0[0x8];
    struct Ref *x8;
    char padC[0x1B4 - 0xC];
    int x1B4;
};

void func_0017FDB8(struct Obj *obj)
{
    if (obj->x8) {
        obj->x8->x0->slot = obj->x1B4;
    }
}
