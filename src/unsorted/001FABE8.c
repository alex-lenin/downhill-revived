/* UNIT 0x001FABE8 0x001FAC20 */
struct Obj { char pad[0x30]; int x30, x34, x38; };
struct Values { int x0, x4, x8; };
extern void func_001FAAA0(struct Obj *);
void func_001FABE8(struct Obj *obj, struct Values *values) {
    obj->x30 = values->x0;
    obj->x34 = values->x4;
    obj->x38 = values->x8;
    func_001FAAA0(obj);
}
