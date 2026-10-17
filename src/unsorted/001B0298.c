/* UNIT 0x001B0298 0x001B0320 */
typedef struct Obj {
    int x0;
    float x4;
    float x8;
    char padC[0x8];
    int x14;
} Obj;

extern int func_00238F48(void);
extern float func_00236088(float current, float target, float a, float b);

void func_001B0298(Obj *obj)
{
    if (obj->x14 != 0 && obj->x0 >= 0) {
        if (func_00238F48() - obj->x0 < 10) {
            obj->x4 = func_00236088(obj->x4, obj->x8, 0.1f, 0.05f);
        } else {
            obj->x4 = func_00236088(obj->x4, 0.0f, 0.1f, 0.05f);
        }
    }
}
