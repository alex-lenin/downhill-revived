/* UNIT 0x001FA9C0 0x001FAA18 */
struct Target { char pad0[0x50]; int x50, x54; char pad58[0x74]; int xCC; };
struct Obj { struct Target *x0, *x4, *x8, *xC; char pad10[0xC]; int x1C, x20, x24, x28; };
void func_001FA9C0(struct Obj *obj) {
    if (obj->x0) obj->x0->xCC = obj->x1C;
    if (obj->x4) obj->x4->xCC = obj->x20;
    if (obj->x8) obj->x8->x50 = obj->x24;
    if (obj->xC) obj->xC->x54 = obj->x28;
}
