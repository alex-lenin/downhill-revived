/* UNIT 0x00190C88 0x00190CE8 */
typedef struct { int flags; int x4; int x8; } Entry;
typedef struct { int x0; Entry entries[1]; } Obj;
void func_00190C88(Obj *obj, int index, int enabled, int bit) {
    if (enabled) {
        obj->entries[index].flags |= 1 << bit;
    } else {
        obj->entries[index].flags ^= obj->entries[index].flags & (1 << bit);
    }
}
