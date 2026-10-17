/* UNIT 0x00143D20 0x00143DB0 */
struct Item {
    char pad[0xF80];
};

struct Obj {
    int count;
    char pad4[0xC];
    struct Item items[1]; /* really `count` items, laid out from 0x10 */
    int current;          /* 0xF90 */
};

extern void func_00143538(struct Item *item, int selected);

void func_00143D20(struct Obj *obj)
{
    int i;

    for (i = 0; i < obj->count; i++) {
        func_00143538(&obj->items[i], obj->current == i);
    }
    obj->current++;
    if (obj->current >= obj->count) {
        obj->current = 0;
    }
}
