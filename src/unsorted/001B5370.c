/* UNIT 0x001B5370 0x001B53C0 */
struct Obj { char pad[0x4C]; int x4C, x50; };
extern void func_001B5AF0(struct Obj *, int, signed char);
extern void func_001B4AF8(struct Obj *);
void func_001B5370(struct Obj *obj, int value) {
    obj->x50 = value;
    obj->x4C = 1;
    func_001B5AF0(obj, 4, 1);
    func_001B5AF0(obj, 8, 1);
    func_001B4AF8(obj);
}
