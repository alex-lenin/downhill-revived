/* UNIT 0x001B97E0 0x001B9860 */
struct Triple { int x0, x4, x8; };
struct Slot { struct Triple x0; char padC[0x44]; };
struct Record { struct Slot slots[21]; int x690; };
struct Obj { char pad[0x10C]; int x10C; int x110; char pad114[8]; struct Triple x11C[31]; int x290[1]; };
extern struct Record D_004225A0[];
void func_001B97E0(struct Obj *obj, int index, int x, int y, int z) {
    struct Triple *entry = &obj->x11C[index];
    entry->x0 = x;
    entry->x4 = y;
    entry->x8 = z;
    if (obj->x110 == 1) {
        int i = (obj->x290[index] >> 5) - 1;
        struct Record *rec = &D_004225A0[i];
        (obj->x10C + rec->slots)->x0 = *entry;
    }
}
