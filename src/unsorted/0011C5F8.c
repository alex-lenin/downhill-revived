/* UNIT 0x0011C5F8 0x0011C668 */
struct Item { char pad[0x7BD0]; };
struct Obj { char pad[0x501A8]; int x501A8; };
extern void func_0012D628(struct Item *);
void func_0011C5F8(struct Obj *obj) {
    int i;
    for (i=0; i<obj->x501A8; i++) {
        struct Item *item = &((struct Item *)obj)[i];
        if (item) func_0012D628(item);
    }
}
