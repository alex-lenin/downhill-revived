/* UNIT 0x001441C0 0x00144238 */
struct Item {
    char pad0[0x110];
};

struct Obj {
    struct Item items[1];
    char pad110[0x7A0 - 0x110];
    int count;
    int x7A4;
};

extern void func_00143F10(struct Obj *obj);
extern void func_00144520(struct Item *item);

void func_001441C0(struct Obj *obj)
{
    int i;

    if (obj->x7A4) {
        func_00143F10(obj);
        obj->x7A4 = 0;
    } else {
        for (i = 0; i < obj->count; i++) {
            func_00144520(&obj->items[i]);
        }
    }
}
