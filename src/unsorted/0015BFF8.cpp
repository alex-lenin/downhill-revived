/* UNIT 0x0015BFF8 0x0015C058 */
class Obj {
public:
    int x0;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(Obj *);
};
void func_0015BFF8(Obj **obj, Obj *next) {
    (*obj)->v4(next);
    next->v3();
    *obj = next;
}
