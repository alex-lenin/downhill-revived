/* UNIT 0x00181730 0x001817E0 */
struct Obj {
    char pad0[0x20]; unsigned int x20, x24, x28;
    char pad1[4]; unsigned int x30, x34, x38;
    char pad2[0x4C-0x3C]; float x4C, x50, x54, x58;
    char pad3[0x80-0x5C]; char x80[0x4B84-0x80]; int x4B84;
};
extern void func_00163970(struct Obj *, int, void *);
void func_00181730(struct Obj *obj) {
    func_00163970(obj, 0, obj->x80);
    obj->x4C = 0.5f;
    obj->x50 = 0.25f;
    obj->x54 = 0.3f;
    obj->x58 = 0.0008f;
    obj->x30 = 0xFF20FF80;
    obj->x34 = 0x2020FF80;
    obj->x38 = 0xFF202080;
    obj->x20 = 0x20FFFF80;
    obj->x24 = 0x20FF2080;
    obj->x28 = 0x20202080;
    obj->x4B84 = 0;
}
