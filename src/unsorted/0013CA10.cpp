/* UNIT 0x0013CA10 0x0013CA48 */
class Child {
public:
    char pad0[0x4F8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3(int, int, int);
};
struct Obj { char pad0[0x500]; Child *x500; };
void func_0013CA10(Obj *obj, int a, int b, int c) {
    Child *child = obj->x500;
    if (child) child->v3(a, b, c);
}


