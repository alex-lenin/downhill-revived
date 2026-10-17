/* UNIT 0x001AD760 0x001AD7C0 */
typedef struct Obj { char pad[0x18]; unsigned char x18; char pad2[11]; float x24; char pad3[12]; float x34; } Obj;
extern float func_00248990(float);
float func_001AD760(Obj *obj, float scale, float offset) {
    float result;
    if (obj->x18) result = func_00248990((obj->x24 + offset) * scale * 3.1415927f) / obj->x34;
    else result = 0.0f;
    return result;
}
