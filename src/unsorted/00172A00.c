/* UNIT 0x00172A00 0x00172A30 */
struct Obj { char pad0[0xC]; unsigned int **xC; char pad10[0x114]; unsigned int x124; };
void func_00172A00(struct Obj *obj) {
    unsigned int *flags;
    flags = *obj->xC;
    *flags = (*flags & 0xFFFC007F) | ((obj->x124 & 0x7FF) << 7);
}
