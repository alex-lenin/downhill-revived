/* UNIT 0x0019DE60 0x0019DE80 */
struct Obj {
    char pad0[0x625];
    unsigned char x625;
};

extern unsigned char D_003EB834[];

int func_0019DE60(struct Obj *obj) {
    if (D_003EB834[0]) {
        obj->x625 = 1;
    }
    return 0;
}
