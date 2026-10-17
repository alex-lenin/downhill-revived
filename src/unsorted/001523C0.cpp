/* UNIT 0x001523C0 0x00152410 */
class Target {
public:
    int x0;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
};
struct Obj { char *x0; int x4; Target *x8; };
extern "C" void func_00136030(void *, int);
void func_001523C0(Obj *obj) {
    if (obj->x8) {
        func_00136030(obj->x0 + 0x40C0, 1);
        obj->x8->v5();
    }
}
