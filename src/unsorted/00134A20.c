/* UNIT 0x00134A20 0x00134AA8 */
struct Obj { char pad0[0x150]; float x150; char pad154[0x480-0x154]; float x480; char pad484[0x4AA-0x484]; short x4AA; char pad4AC[4]; unsigned long x4B0; };
extern void func_002318C0(void *);
extern void func_00169EB0(void *);
extern void func_00134AC0(void *);
extern void func_00134AA8(void *);
struct Obj *func_00134A20(struct Obj *obj) {
    func_002318C0(obj);
    func_00169EB0((char *)obj + 0x160);
    func_00134AC0((char *)obj + 0x2E0);
    func_00134AC0((char *)obj + 0x360);
    func_00134AA8((char *)obj + 0x3E0);
    obj->x480 = 1.0f;
    obj->x150 = 100.0f;
    obj->x4B0 |= 2;
    obj->x4B0 &= ~4;
    obj->x4B0 &= ~8;
    obj->x4AA = 1;
    return obj;
}
