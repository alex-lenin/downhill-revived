/* UNIT 0x001432C0 0x00143330 */
struct Item { char pad[0x170]; };
struct Obj { int x0, x4; char pad[0x68]; struct Item items[1]; };
extern void func_00141CD8(struct Item *, int);
extern void func_001D2F08(void *);
void func_001432C0(struct Obj *obj) {
    int i;
    for (i=0; i<obj->x4; i++) {
        func_00141CD8(&obj->items[i], i);
    }
    obj->x0 = 1;
    func_001D2F08((char *)obj + 0xF70);
}
