/* UNIT 0x001DC2D8 0x001DC328 */
struct Entry { char pad0[0x34]; int *x34; char pad38[0x28]; };
struct Obj { struct Entry entries[100]; char pad2580[0x10]; int *x2590; char pad2594[0x30]; int x25C4; };
void func_001DC2D8(struct Obj *obj, int *data) {
    int flags = *data;
    if ((flags & 0xFFFC0000) == 0x75340000) {
        obj->x2590 = data;
        return;
    }
    if ((flags & 0x3F) == 3) {
        obj->entries[obj->x25C4].x34 = data;
        obj->x25C4++;
    }
}
