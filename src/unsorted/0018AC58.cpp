/* UNIT 0x0018AC58 0x0018ACB8 */
struct VTable { char pad[0x40]; short delta; short pad42; void (*method)(void *, void *); };
struct Obj { char pad0[0x4F8]; struct VTable *x4F8; char pad1[0x610-0x4FC]; unsigned char x610; };
struct Item { char pad[0x1C]; short x1C; };
extern void func_00187438(struct Obj *, void *);
void func_0018AC58(struct Obj *obj, int index) {
    struct Item *item = (struct Item *)((index * 16) + (unsigned int)obj);
    struct VTable *vt;
    bool matches = item->x1C == 0x1F;
    if (matches) {
        vt = obj->x4F8;
        vt->method((char *)obj + vt->delta, item);
    } else {
        func_00187438(obj, item);
        obj->x610 = 0;
    }
}
