/* UNIT 0x0013C300 0x0013C338 */
class Child {
public:
    char pad0[0x4F8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
};
struct Obj { char pad0[0x500]; Child *x500; };
void func_0013C300(Obj *obj) {
    Child *child = obj->x500;
    if (child) child->v5();
}


