/* UNIT 0x00122AA0 0x00122CF0 */
typedef struct Stats { char pad[0xD4]; float xD4; } Stats;
typedef struct State { int x0; unsigned long *x4; char pad8[8]; int x10; char pad14[0x21C]; Stats *x230; } State;
typedef struct Obj { virtual void dummy(); int x0; char pad4[0x14]; int x18; char pad1C[0x20]; int x3C; char pad40[0x28]; char x68[0xC8]; State *x130; int x134; char pad138[0x14]; void (Obj::*x14C)(); } Obj;
extern "C" int func_00238F60(void);
extern "C" int func_00165CD8(int);
extern "C" void func_00127098(Obj *), func_00165560(int), func_00127170(Obj *, int);
extern "C" float func_001ACBA0(void *), func_00235EA8(float,float,float,float), func_00122DC8(Obj *);
extern "C" void func_001ACAE0(void *,float), func_00204358(int *,int);
extern "C" void func_00122AA0(Obj *obj) {
    long entry;
    void (*method)(void *);
    int offset;
    float distance;
    int *handle;
    obj->x18 += func_00238F60();
    if (obj->x3C) {
        obj->x3C -= func_00238F60();
        if (obj->x3C < 0) obj->x3C = 0;
    }
    if (obj->x134) {
        if (func_00165CD8(obj->x134)) func_00127098(obj);
        else func_00165560(obj->x134);
        func_001ACAE0(obj->x68, func_00235EA8(func_001ACBA0(obj->x68), 0.5f,0.1f,0.0001f));
    } else if (obj->x0) {
        func_00127170(obj,1);
    } else {
        if (obj->x130->x230->xD4 < 100.0f) *obj->x130->x4 |= 0x0200000000000000UL;
        else *obj->x130->x4 &= ~0x0200000000000000UL;
        (obj->*obj->x14C)();
    }
    handle = &obj->x130->x10;
    if (*handle) {
        distance = func_00122DC8(obj);
        if (distance < 1600.0f) { func_00204358(handle,0); return; }
        if (distance < 6400.0f) { func_00204358(handle,1); return; }
        if (distance < 25600.0f) { func_00204358(handle,2); return; }
        func_00204358(handle,3);
    }
}
