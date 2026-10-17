/* UNIT 0x00152468 0x001524B0 */
struct Target {
    int x0;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6(int);
};
struct Obj { int x0, x4; Target *x8; };
void func_00152468(Obj *obj, int arg1) {
    if (obj->x8) {
        obj->x8->v6(arg1);
        obj->x8 = 0;
    }
}

