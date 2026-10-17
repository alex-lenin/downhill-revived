/* UNIT 0x001DF820 0x001DF8C0 */
struct Item {
    int id;
    char pad4[0x2C];
};

struct Global {
    char pad0[0x260];
    struct Item items[47];
    char padB30[0x10];
    int itemCount;
};

struct Obj {
    char pad0[0x18C];
    int x18C;
    int x190;
};

extern struct Global D_00398C40;

void func_001DF820(struct Obj *obj)
{
    int best = 0;
    int i;
    int id;

    for (i = 0; i < D_00398C40.itemCount; i++) {
        id = D_00398C40.items[i].id;
        if (id >= 0x6A5 && id <= 0x6A8 && best < id) {
            best = id;
        }
    }
    if (best == 0x6A5) {
        obj->x18C = 0;
    } else if (best == 0x6A6) {
        obj->x18C = 1;
    } else if (best == 0x6A7) {
        obj->x18C = 2;
    } else if (best == 0x6A8) {
        obj->x18C = 3;
    } else {
        obj->x18C = obj->x190;
    }
}
