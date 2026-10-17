/* UNIT 0x001919E8 0x00191A28 */
typedef struct Obj { char pad0[8]; unsigned int **x8; char padC[0x74]; unsigned int x80; } Obj;
void func_001919E8(Obj *obj) {
    unsigned int **link;
    unsigned int *flags;
    link = obj->x8;
    if (link != 0) {
        flags = *link;
        *flags = (*flags & 0xFFFC007F) | ((obj->x80 & 0x7FF) << 7);
    }
}
