/* UNIT 0x0012ACC8 0x0012AD60 */
typedef struct { int x0, x4, x8, xC; } Value;
typedef struct { char pad[0x4C]; Value x4C; char pad2[0xF8]; float x154, x158; char pad3[0x1C]; int x178; int x17C; int x180; } Obj;
extern float func_00235EA8(float, float, float, float);
extern void func_00203E10(Value *, float);
void func_0012ACC8(Obj *obj) {
    Value value;
    float result;
    if (obj->x178) {
        obj->x180 = 10;
        obj->x178 = 0;
        if (obj->x154 < obj->x158) {
            result = func_00235EA8(obj->x154, obj->x158, 0.08f, 0.1f);
            obj->x154 = result;
            value = obj->x4C;
            func_00203E10(&value, result);
        }
    }
}
