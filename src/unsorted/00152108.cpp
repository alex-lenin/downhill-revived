/* UNIT 0x00152108 0x00152168 */
class Component {
public:
    int x0;
    virtual int method1();
    virtual int method2();
    virtual int method3();
    virtual int method4();
};
struct Obj {
    char pad0[0x6330];
    Component x6330;
    char pad6338[0x186C];
    int x7BA4;
};
extern "C" void func_0012FE38(Obj *, Component *);
extern "C" void func_00152108(int value, Obj *obj) {
    Component *component = &obj->x6330;
    obj->x7BA4 = value;
    if (component->method4()) {
        func_0012FE38(obj, component);
    } else {
        obj->x7BA4 = 0;
    }
}
