/* UNIT 0x001528C0 0x00152918 */
struct Obj { char pad[8]; float x8, xC, x10; char pad14[12]; unsigned char x20, x21; };
extern void func_001524B0(struct Obj *, int);
void func_001528C0(struct Obj *obj, int value) {
    func_001524B0(obj, value);
    obj->x8 = 1.2f;
    obj->xC = 40.0f;
    obj->x10 = 100.0f;
    obj->x20 = 1;
    obj->x21 = 0;
}
