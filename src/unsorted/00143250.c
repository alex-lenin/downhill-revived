/* UNIT 0x00143250 0x001432C0 */
struct Item { char pad[0x170]; };
struct Obj { int x0, x4; char pad[0x68]; struct Item items[1]; };
extern void func_00141CA8(struct Item *);
void func_00143250(struct Obj *obj) {
    int i;
    if (obj->x0) {
        obj->x0 = 1;
        for (i = 0; i < obj->x4; i++) {
            func_00141CA8(&obj->items[i]);
        }
    }
}
