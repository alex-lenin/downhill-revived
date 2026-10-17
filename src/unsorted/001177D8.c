/* UNIT 0x001177D8 0x001177F8 */
struct Obj { char pad[0x10000]; int x10000; float x10004; int x10008; };
void func_001177D8(struct Obj *obj) {
    obj->x10000 = 0;
    obj->x10004 = 10.0f;
    obj->x10008 = 0;
}
