/* UNIT 0x0018F160 0x0018F1D8 */
struct VEntry {
    short delta;
    short pad;
    void (*fn)(void *);
};

struct Item {
    char pad0[0x4B9];
    char x4B9;
    char pad4BA[0x4F8 - 0x4BA];
    struct VEntry *vtbl;
};

struct List {
    char pad0[0x500];
    struct Item *items[12];
    int count;
    char pad534[0x538 - 0x534];
    char x538[0x554 - 0x538];
    char x554[1];
};

extern void func_0018E4C0(struct Item *, void *);
extern void func_0018E4D8(struct Item *, void *);

void func_0018F160(struct List *list, struct Item *item)
{
    struct VEntry *e;

    list->items[list->count] = item;
    item->x4B9 = 0;
    e = &item->vtbl[14];
    e->fn((char *)item + e->delta);
    func_0018E4C0(item, list->x554);
    func_0018E4D8(item, list->x538);
    list->count++;
}
