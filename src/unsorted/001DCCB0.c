/* UNIT 0x001DCCB0 0x001DCCD8 */
struct Obj {
    char pad0[0x34];
    int *x34;
};

void func_001DCCB0(struct Obj *obj) {
    int *p = obj->x34;
    if (p != 0) {
        *p = (*p & ~0x3F) | 3;
    }
}
