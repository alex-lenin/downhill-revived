/* UNIT 0x00234BD8 0x00234D08 */
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct Item { char pad[5]; u8 flags; char rest[6]; } Item;
typedef struct Obj { char pad[0x1d4]; Item *x1d4; char *x1d8; } Obj;
void func_00234BD8(Obj *obj, u32 *node, u32 depth) {
    u32 *entry = node + 1;
    u32 level = 0;
    u32 i;
    while (level < depth && *node) {
        for (i = 0; i < *node - 1; i++) {
            func_00234BD8(obj, (u32 *)(obj->x1d8 + entry[1]), depth - level - 1);
            entry += 2;
        }
        level++;
        node = (u32 *)(obj->x1d8 + entry[1]);
        entry = node + 1;
    }
    {
    int *start = (int *)(node + 1);
    int *indices;
    i = 0;
    if (*node) {
    indices = start;
    do {
        Item *item = (Item *)(*indices * 12 + (int)obj->x1d4);
        u8 flags = item->flags;
        if (flags & 1) item->flags = flags & 0xfe;
        i++; indices++;
    } while (i < *node);
    }
    }
}
