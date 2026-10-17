/* UNIT 0x0017F8B8 0x0017FA30 */
struct Table {
    unsigned int **entries[131];
};

struct Entry {
    char pad0[0x1E8];
    unsigned char x1E8;
};

struct Item {
    char pad0[0x1B8];
    unsigned char x1B8;
    char pad1B9[0x220 - 0x1B9];
};

struct Obj {
    char pad0[0x30];
    struct Item items[40];
    char pad5530[0x5530 - 0x30 - 40 * 0x220];
    unsigned int **x5530[131];
    int x573C; /* item count */
    int x5740;
    int x5744;
    int x5748; /* current item */
    char pad574C[0x576C - 0x574C];
    int x576C;
};

extern void func_0017F160(struct Obj *obj);
extern int func_0017F8A8(struct Item *item);
extern void func_00180000(struct Item *item);
extern void func_001D33B0(void *p, unsigned int v);

void func_0017F8B8(struct Obj *obj)
{
    int index;
    int offset;
    int i;
    struct Item *item;
    struct Table *table;

    if (obj->x5744) {
        func_0017F160(obj);
        obj->x5744 = 0;
    } else {
        {
            int previous = obj->x5748;
            obj->x5748 = previous + 1;
            ((struct Entry *)((char *)obj + previous * 0x220))->x1E8 = 0;
        }
        if (obj->x5748 >= obj->x573C) {
            obj->x5748 = 0;
        }
        ((struct Entry *)((char *)obj + obj->x5748 * 0x220))->x1E8 = 1;
        i = 0;
        while (1) {
            /* set inside the loop: loop.c hoists it past the loop note */
            table = (struct Table *)obj->x5530;
            if (i >= obj->x573C) {
                break;
            }
            index = obj->x5748 + i;
            if (index >= obj->x573C) {
                index -= obj->x573C;
            }
            offset = index * 0x220;
            if (func_0017F8A8((struct Item *)((char *)obj + (offset + 0x30)))) {
                obj->x5748 = index;
                ((struct Entry *)(offset + (unsigned int)obj))->x1E8 = 1;
                break;
            }
            i++;
        }
        i = 0;
        if (obj->x573C > 0) {
            item = obj->items;
            do {
                func_00180000(item);
                i++;
                item++;
            } while (i < obj->x573C);
        }
        {
            int current = obj->x576C;
            if (table->entries[current]) {
                func_001D33B0((char *)obj + current * 0x220 + 0x1F0, **table->entries[current] >> 18);
            }
        }
        obj->x576C++;
        if (obj->x576C >= obj->x573C) {
            obj->x576C = 0;
        }
    }
}
