/* UNIT 0x001385B0 0x00138650 */
struct Obj { char pad[0x150]; float x150; char pad2[0x428-0x154]; float x428; char pad3[0x4A0-0x42C]; float x4A0; };
extern float D_007684A0[];
extern float func_00138580(struct Obj *);
void func_001385B0(struct Obj *obj) {
    float value = D_007684A0[0] * (obj->x428 + (obj->x150 - 100.0f) * 0.001f) * 1.4666667f;
    if (value > 0.0f) obj->x4A0 = func_00138580(obj) / (value * 1.4666667f);
    else obj->x4A0 = 0.0f;
}
