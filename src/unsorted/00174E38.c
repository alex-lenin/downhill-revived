/* UNIT 0x00174E38 0x001750A8 */
typedef struct Back {
    char pad0[0x4ECC];
    struct Obj *x4ECC;
} Back;

typedef struct Obj {
    Back *x0;
    char pad4[4];
    unsigned char x8;
    char pad9[7];
    float x10;
    float x14;
    float x18;
    char pad1C[4];
    unsigned char x20;
    unsigned char x21;
} Obj;

extern float func_00236088(float, float, float, float);
extern void func_001750A8(Obj *);

int func_00174E38(Obj *obj) {
    if (obj->x0->x4ECC == obj) {
        return 1;
    }
    if (obj->x21 == 0) {
        obj->x14 = func_00236088(obj->x14, 1.0f, 0.2f, 0.1f);
        obj->x10 = func_00236088(obj->x10, -0.8f, 0.2f, 0.1f);
        if (obj->x10 <= -0.7f) {
            obj->x21 = 1;
        }
        func_001750A8(obj);
        return 1;
    }
    if (obj->x20 == 0) {
        obj->x14 = func_00236088(obj->x14, 1.0f, 0.2f, 0.1f);
        obj->x10 = func_00236088(obj->x10, 1.0f, 0.22000001f, 0.12000001f);
        obj->x18 = func_00236088(obj->x18, 1.0f, 0.2f, 0.1f);
        if (obj->x10 >= 1.0f) {
            obj->x20 = 1;
        }
        func_001750A8(obj);
        return 1;
    }
    obj->x14 = func_00236088(obj->x14, 0.0f, 0.02f, 0.01f);
    obj->x18 = func_00236088(obj->x18, 0.0f, 0.02f, 0.01f);
    obj->x10 = func_00236088(obj->x10, 0.0f, 0.02f, 0.025f);
    func_001750A8(obj);
    obj->x8 = (obj->x10 != 0.0f || obj->x18 != 0.0f || obj->x14 != 0.0f);
    return (obj->x10 != 0.0f || obj->x18 != 0.0f || obj->x14 != 0.0f);
}
