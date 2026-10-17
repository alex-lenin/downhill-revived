/* UNIT 0x00163BE0 0x00163C58 */
struct Entry {
    char pad0[0x14];
    unsigned short flags;
};

struct Obj {
    char pad0[0x5C];
    int count;
};

extern struct Entry *func_00163D40(struct Obj *obj, int index);

/* Returns the index of the next entry after `start` with bit 0 of flags set, or 0. */
int func_00163BE0(struct Obj *obj, int start)
{
    int i;
    int found;

    i = 0;
    /* "> i - 1" (not "> -1"/">= 0") keeps the retail slt -1 < start + movn */
    if (++start > i - 1) {
        i = start;
    }
    found = 0;
    for (; i < obj->count; i++) {
        if (func_00163D40(obj, i)->flags & 1) {
            found = i;
            break;
        }
    }
    return found;
}
