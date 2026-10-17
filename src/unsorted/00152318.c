/* UNIT 0x00152318 0x001523C0 */
struct Obj {
    char pad0[0x8];
    void *x8;
    char x0C[0x28];
    char x34[0x28];
    char x5C[0x28];
    char x84[0x2C];
    char xB0[0x28];
};

extern int func_001525B8(void *p);

int func_00152318(struct Obj *obj) {
    int found = 0;

    if (func_001525B8(obj->x0C)) {
        obj->x8 = obj->x0C;
        found = 1;
    } else if (func_001525B8(obj->x34)) {
        obj->x8 = obj->x34;
        found = 1;
    } else if (func_001525B8(obj->x5C)) {
        obj->x8 = obj->x5C;
        found = 1;
    } else if (func_001525B8(obj->x84)) {
        obj->x8 = obj->x84;
        found = 1;
    } else if (func_001525B8(obj->xB0)) {
        obj->x8 = obj->xB0;
        found = 1;
    }
    return found;
}
