/* UNIT 0x001B13A0 0x001B13E0 */
struct Child { char pad[0x30]; float x30; int x34, x38; };
struct Obj { char pad0[0xD4]; struct Child *xD4, *xD8; char padDC[0x28]; float x104, x108; };
void func_001B13A0(struct Obj *obj) {
    obj->xD4->x30 = obj->x104;
    obj->xD4->x34 = 0;
    obj->xD4->x38 = 0;
    obj->xD8->x30 = obj->x108;
    obj->xD8->x34 = 0;
    obj->xD8->x38 = 0;
}
