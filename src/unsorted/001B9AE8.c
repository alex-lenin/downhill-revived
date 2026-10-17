/* UNIT 0x001B9AE8 0x001B9B70 */
struct Entry { int x0, x4, x8; };
struct Obj { char pad[0x10C]; int x10C, x110; char pad114[0xA4]; struct Entry entries[1]; };
struct GlobalEntry { char pad[0x18]; struct Entry entry; char tail[0x2C]; };
struct GlobalRow { struct GlobalEntry entries[21]; char tail[4]; };
extern struct GlobalRow D_004225A0[];
void func_001B9AE8(struct Obj *object, int value, int index, int a, int b) {
    int row = (value >> 5) - 1;
    struct Entry *entry = &object->entries[index];
    entry->x0 = row;
    entry->x8 = b;
    if (object->x10C == 10) entry->x4 = 0;
    else entry->x4 = a;
    if (object->x110 == 1) {
        int mode = object->x10C * 0x50;
        int base = row * 0x694 + (int)D_004225A0;
        struct GlobalEntry *dest = (struct GlobalEntry *)(mode + base);
        dest->entry = *entry;
    }
}
