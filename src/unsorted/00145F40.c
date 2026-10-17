/* UNIT 0x00145F40 0x00145FE0 */
struct Obj { char pad[0x20]; unsigned int x20,x24; char pad2[8]; unsigned int x30,x34; char pad3[0x4C-0x38]; float x4C,x50,x54,x58; char pad4[0x2084-0x5C]; int x2084; };
extern void func_00163970(struct Obj *, int, void *);
void func_00145F40(struct Obj *obj) {
    func_00163970(obj, 0, (char *)obj + 0x80);
    obj->x4C = 0.4f;
    obj->x50 = 0.2f;
    obj->x54 = 0.3f;
    obj->x58 = 0.0008f;
    obj->x30 = 0xFF20FF80;
    obj->x34 = 0x2020FF80;
    obj->x20 = 0x20FFFF80;
    obj->x24 = 0x20202080;
    obj->x2084 = 0;
}
