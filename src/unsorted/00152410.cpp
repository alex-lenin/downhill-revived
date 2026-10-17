/* UNIT 0x00152410 0x00152468 */
struct Obj;
class Target {
public:
    int x0;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10(Obj *);
};
struct Obj { char *x0; int x4; Target *x8; };
extern void func_001AC578(void *);
void func_00152410(Obj *obj) {
    if (obj->x8) {
        func_001AC578(obj->x0 + 0x1e40);
        obj->x8->v10(obj);
    }
}
