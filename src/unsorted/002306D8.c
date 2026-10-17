/* UNIT 0x002306D8 0x00230700 */
struct Obj { char pad0[0x50004]; int x50004; };
void func_002306D8(struct Obj *obj, int amount) {
    int count = obj->x50004;
    obj->x50004 = count - (count >= amount ? amount : count);
}
