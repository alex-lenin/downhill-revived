/* UNIT 0x001A9038 0x001A9058 */
struct Obj {
    char pad0[0xC];
    int xC;
    char pad10[0x4];
    int x14;
    char pad18[0xC];
    int x24;
    char pad28[0x9C8 - 0x28];
    int x9C8;
};

void func_001A9038(struct Obj *obj) {
    obj->x24 = 0;
    obj->xC = obj->x14 + 2000;
    obj->x9C8 = -1000;
}
