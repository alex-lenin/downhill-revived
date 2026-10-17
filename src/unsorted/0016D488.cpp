/* UNIT 0x0016D488 0x0016D530 */
struct Obj;
typedef int (Obj::*Handler)();

struct Obj {
    char pad0[0x28];
    int x28;
    char pad2C[0x130 - 0x2C];
    int x130;
    char pad134[0x154 - 0x134];
    Handler x154;

    void func_00122AA0();
    virtual void Dummy();
};

struct Mgr {
    void func_0016D488(Obj *obj);
};

void Mgr::func_0016D488(Obj *obj) {
    if (obj->x130) {
        if ((unsigned char)(obj->*obj->x154)()) {
            obj->x28 = 1;
        } else {
            obj->x28 = 0;
        }
        obj->func_00122AA0();
    }
}
