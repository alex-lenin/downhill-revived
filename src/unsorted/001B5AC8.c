/* UNIT 0x001B5AC8 0x001B5AF0 */
struct Obj {
    char pad0[0x55];
    char x55;
    char pad56[0x61 - 0x56];
    char x61;
};

extern void func_001B5788(struct Obj *o);

void func_001B5AC8(struct Obj *a) {
    struct Obj *o = a;
    o->x61 = 1;
    o->x55 = 0;
    func_001B5788(o);
}
