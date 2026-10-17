/* UNIT 0x0012AC28 0x0012ACC8 */
struct Vec { float x, y, z, w; };
struct Obj { char pad[0x4C]; struct Vec x4C; char pad2[0x150-0x5C]; float x150; char pad3[0x178-0x154]; int x178; };
extern float func_00203C28(struct Vec *);
extern int func_00222BD8(int, int);
void func_0012AC28(struct Obj *obj) {
    struct Vec vec = obj->x4C;
    float value = func_00203C28(&vec);
    if (value < obj->x150 || (value < 0.0f && func_00222BD8(0,100) == 42)) obj->x178 = 1;
    obj->x150 = value;
}
