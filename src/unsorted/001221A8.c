/* UNIT 0x001221A8 0x00122258 */
typedef struct { char pad[0x20]; unsigned int x20, x24, x28, x2C, x30, x34, x38, x3C; char pad2[0xC]; float x4C, x50, x54, x58; char pad3[0x4B28]; int x4B84; } Obj;
extern void func_00163970(Obj *, int, void *);
void func_001221A8(Obj *obj) {
    func_00163970(obj, 0, (char *)obj + 0x80);
    obj->x4C = 0.4f;
    obj->x50 = 0.2f;
    obj->x54 = 0.3f;
    obj->x58 = 0.0008f;
    obj->x30 = 0xFF20FF80;
    obj->x34 = 0x20202080;
    obj->x38 = 0xFF20FF80;
    obj->x3C = 0xFF20FF80;
    obj->x20 = 0x20FFFF80;
    obj->x24 = 0x2020FF80;
    obj->x28 = 0x20FFFF80;
    obj->x2C = 0x20FFFF80;
    obj->x4B84 = 0;
}
