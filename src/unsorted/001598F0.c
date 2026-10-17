/* UNIT 0x001598F0 0x00159930 */
struct Flags { unsigned int low : 7; unsigned int value : 11; unsigned int high : 14; };
struct Obj { char pad0[8]; struct Flags **x8; char padC[0x1E2]; short x1EE; };
void func_001598F0(struct Obj *obj) {
    struct Flags **link;
    struct Flags *flags;
    link = obj->x8;
    if (link != 0) {
        flags = *link;
        flags->value = obj->x1EE;
    }
}
