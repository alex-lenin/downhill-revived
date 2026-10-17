/* UNIT 0x001B0870 0x001B08D0 */
struct Part { char pad[0x100]; unsigned char x100; char tail[15]; };
struct Parent {
    char pad[0x3D10]; Part x3D10; Part x3E20;
    Part *left() { return &x3D10; }
    Part *right() { return &x3E20; }
};
struct Obj { char pad[0x10]; Parent *x10; int x14; float x18; };
void func_001B0870(Obj *obj, int value) {
    Parent *parent = obj->x10;
    obj->x14 = value;
    parent->left()->x100 = obj->x18 < 0.1f;
    parent->right()->x100 = obj->x18 > -0.1f;
}
