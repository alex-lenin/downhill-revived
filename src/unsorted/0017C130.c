/* UNIT 0x0017C130 0x0017C168 */
struct Child { char pad0[0x10]; float x10; };
struct Table { char pad0[0x20]; struct Child *x20[1]; };
struct Obj { char pad0[0xf84]; struct Table *xf84; char padf88[0x300]; float x1288; char pad128c[0x88]; int x1314; char pad1318[0x3e]; unsigned char x1356; };
void func_0017C130(struct Obj *obj) {
    struct Child *child;
    child = obj->xf84->x20[obj->x1356];
    child->x10 += obj->x1288;
    obj->x1314--;
}
