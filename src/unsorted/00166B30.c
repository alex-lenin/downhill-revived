/* UNIT 0x00166B30 0x00166BB0 */
struct Obj { char pad[0x120]; int x120; };
extern void func_001AC880(void *, float);
void func_00166B30(struct Obj *obj, float value) {
    func_001AC880((char *)obj + 0x1B8, value);
    func_001AC880((char *)obj + 0x280, value);
    if (obj->x120 == 0) {
        func_001AC880((char *)obj + 0x21C, value);
        func_001AC880((char *)obj + 0x2E4, value);
        func_001AC880((char *)obj + 0x348, value);
        func_001AC880((char *)obj + 0x3AC, value);
    }
}
