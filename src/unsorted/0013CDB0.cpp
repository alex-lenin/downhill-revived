/* UNIT 0x0013CDB0 0x0013CE20 */
struct Entry { char pad[6]; unsigned char x6; char pad2[9]; unsigned char active() { return x6; } };
struct Vtable { char pad[8]; short delta; short pad2; void (*fn)(void *); };
struct Obj { int x0; char pad[20]; struct Entry entries[78]; char pad2[0x4F8-0x4F8]; struct Vtable *x4F8; char pad3[0x5DC-0x4FC]; int x5DC; };
void func_0013CDB0(struct Obj *obj) {
    int i;
    obj->x4F8->fn((char *)obj + obj->x4F8->delta);
    for (i = 0; i < obj->x5DC; i++) {
        if (obj->entries[i].active()) { obj->x0 = i; return; }
    }
    obj->x0 = 0;
}
