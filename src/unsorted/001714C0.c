/* UNIT 0x001714C0 0x00171538 */
struct Item {
    char pad0[0x210];
};

struct Obj {
    struct Item items[1];
    char pad210[0x29BC - 0x210];
    int count;
    int x29C0;
};

extern void func_00171310(struct Obj *obj);
extern void func_00171A18(struct Item *item);

void func_001714C0(struct Obj *obj)
{
    int i;

    if (obj->x29C0) {
        func_00171310(obj);
        obj->x29C0 = 0;
    } else {
        for (i = 0; i < obj->count; i++) {
            func_00171A18(&obj->items[i]);
        }
    }
}
