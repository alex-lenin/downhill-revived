/* UNIT 0x00230DC8 0x00230DF0 */
struct Obj { char pad0[0x808C]; int x808C; int x8090; };
extern void func_00230BD0(struct Obj *obj);
void func_00230DC8(struct Obj *obj, int value) {
    obj->x808C = value;
    obj->x8090 = 0;
    func_00230BD0(obj);
}
