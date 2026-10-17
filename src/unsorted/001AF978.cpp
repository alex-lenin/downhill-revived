/* UNIT 0x001AF978 0x001AFA88 */
struct Value { float x, y, z, w; };

struct Obj {
    char pad0[0x60];
    Value timer;
    char *x70;
    float x74;
};

extern bool func_00204230(Value *);
extern void func_00203DC8(Value *, int);
extern void func_00202E38(Value *, int, int, float);
extern float func_001385A8(void *);
extern int func_001577B0(void *);
extern void func_001AC778(Obj *, float);

void func_001AF978(Obj *obj)
{
    Value a;
    Value b;
    float scale;
    struct { unsigned char v; } expired;

    if (obj->x70) {
        a = obj->timer;
        expired.v = !func_00204230(&a);
        if (expired.v) {
            b = obj->timer;
            func_00203DC8(&b, 1);
            func_00202E38(&obj->timer, 1, 1, 0.0f);
            return;
        }
        scale = obj->x74 * func_001385A8(obj->x70 + 0x40C0);
        if (scale < 0.3f) {
            scale = 0.0f;
        }
        if (func_001577B0(obj->x70 + 0x4EE0)) {
            scale = 0.0f;
        }
        func_001AC778(obj, scale);
    }
}
