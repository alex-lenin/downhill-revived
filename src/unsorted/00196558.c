/* UNIT 0x00196558 0x00196590 */
struct State {
    int x0;
    char x4;
    char pad5[0xB];
    int x10, x14, x18, x1C;
};
struct Obj {
    int x0, x4;
    struct State *x8;
};
void func_00196558(struct Obj *obj) {
    obj->x4 = 0;
    obj->x8->x4 = 0;
    obj->x8->x0 = 0;
    obj->x8->x10 = 0;
    obj->x8->x14 = 0;
    obj->x8->x18 = 0;
    obj->x8->x1C = 0;
}
