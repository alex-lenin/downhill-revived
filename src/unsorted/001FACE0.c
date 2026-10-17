/* UNIT 0x001FACE0 0x001FAD18 */
struct Obj { char pad0[8]; int x8; char padC[4]; float x10, x14, x18; };
void func_001FACE0(struct Obj *obj) {
    obj->x8 = 0;
    obj->x10 = 1.5f;
    obj->x14 = 0.3f;
    obj->x18 = -0.1f;
}
