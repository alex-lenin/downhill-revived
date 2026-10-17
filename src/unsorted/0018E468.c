/* UNIT 0x0018E468 0x0018E4C0 */
struct Obj;
struct Entry { short delta; short pad; void (*call)(void *); };
struct VTable { struct Entry slots[4]; };
struct Target { char pad[0x4f8]; struct VTable *x4F8; };
struct Link { int x0; struct Target *x4; };
struct Obj { char pad[0x4ac]; struct Link *x4AC; int x4B0; int x4B4; };
extern char D_003F52D0[];
extern void func_001904B8(void *, struct Link *);
void func_0018E468(struct Obj *obj) {
    struct Link *link;
    struct Target *target = 0;
    struct VTable *table;
    obj->x4B4 = 0;
    link = obj->x4AC;
    if (link) target = link->x4;
    func_001904B8(D_003F52D0, link);
    if (target) {
        table = target->x4F8;
        table->slots[3].call((char *)target + table->slots[3].delta);
    }
}
