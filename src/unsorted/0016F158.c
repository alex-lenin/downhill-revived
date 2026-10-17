/* UNIT 0x0016F158 0x0016F198 */
struct Packet {
    unsigned int x0_0 : 7;
    unsigned int slot : 11;
    unsigned int x0_18 : 14;
};

struct Ref {
    struct Packet *x0;
};

struct Obj {
    char pad0[0x1F0];
    struct Ref *x1F0;
    char pad1F4[0x230 - 0x1F4];
    int x230;
};

void func_0016F158(struct Obj *obj)
{
    if (obj->x1F0) {
        obj->x1F0->x0->slot = obj->x230;
    }
}
