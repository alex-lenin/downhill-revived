/* UNIT 0x001481E8 0x00148260 */
struct Slot {
    char pad0[0x380];
};

struct Obj {
    struct Slot slots[4];
    int xE00;
    char padE04[0xC];
};

extern void func_00254204(void *dst, int value, int size);
extern void func_00148310(struct Slot *slot, int index);

void func_001481E8(struct Obj *obj)
{
    int i;

    func_00254204(obj, 0, sizeof(struct Obj));
    for (i = 0; i < 4; i++) {
        func_00254204(&obj->slots[i], 0, sizeof(struct Slot));
        func_00148310(&obj->slots[i], i);
    }
    obj->xE00 = 0;
}
