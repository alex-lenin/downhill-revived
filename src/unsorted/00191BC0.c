/* UNIT 0x00191BC0 0x00191FC0 */
typedef struct Value { int x[4]; } Value;
typedef struct Obj {
    char pad0[4]; int x4;
    char pad8[0x10];
    Value x18, x28, x38;
    char pad48[0x18];
    int x60; float x64; char pad68[4]; float x6C;
    char pad70[4]; int x74; char pad78[4]; int x7C;
} Obj;
extern void func_00203CB8(Value *);
extern float func_00203C28(Value *);
extern unsigned char func_00204230(Value *);
extern void func_00203DC8(Value *, int);
extern void func_00203E10(Value *, float);
extern void func_00202E38(Value *, int, int, float);

void func_00191BC0(Obj *obj, int mode) {
    Value v;
    Value a;
    Value b;
    int flag;
    if (obj->x4 != mode) {
        if (obj->x18.x[0]) { v = obj->x18; func_00203CB8(&v); }
        if (obj->x28.x[0]) { v = obj->x28; func_00203CB8(&v); }
        if (obj->x38.x[0]) { v = obj->x38; func_00203CB8(&v); }
    }
    obj->x4 = mode;
    obj->x74 = 0;
    obj->x7C = 0;
    switch (mode) {
    case 1:
        obj->x60 = 0;
        obj->x64 = 1.4666667f;
        flag = 0;
        v = obj->x18;
        if (func_00203C28(&v) >= 1.0f || (a = obj->x18, !func_00204230(&a))) flag = 1;
        if ((unsigned char)flag) {
            b = obj->x18;
            func_00203DC8(&b, 0);
            func_00202E38(&obj->x18, 1, 1, 150.0f);
        }
        break;
    case 2:
    case 4:
        flag = 0;
        v = obj->x28;
        if (func_00203C28(&v) >= 1.0f || (a = obj->x28, !func_00204230(&a))) flag = 1;
        if ((unsigned char)flag) {
            b = obj->x28;
            func_00203DC8(&b, 1);
            b = obj->x28;
            func_00203E10(&b, obj->x6C);
            func_00202E38(&obj->x28, 1, 1, 150.0f);
        }
        break;
    case 3:
        flag = 0;
        v = obj->x38;
        if (func_00203C28(&v) >= 1.0f || (a = obj->x38, !func_00204230(&a))) flag = 1;
        if ((unsigned char)flag) {
            b = obj->x38;
            func_00203DC8(&b, 1);
            b = obj->x38;
            func_00203E10(&b, 2.0f);
            func_00202E38(&obj->x38, 1, 1, 150.0f);
        }
        break;
    }
}
