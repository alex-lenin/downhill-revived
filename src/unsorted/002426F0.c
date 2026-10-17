/* UNIT 0x002426F0 0x00242798 */
typedef struct Obj { char pad0[0x44]; int x44, x48, x4C; } Obj;
void func_002426F0(int *out0,int *out1,int *out2,int *out3,Obj *obj,int value) {
    value = (value + obj->x48 - obj->x4C - 0x400) % obj->x48;
    value = value / 1024 * 1024;
    if (obj->x48 - obj->x4C >= value) {
        *out0 = obj->x44 + obj->x4C;
        *out1 = value;
        *out2 = 0;
        *out3 = 0;
    } else {
        *out0 = obj->x44 + obj->x4C;
        *out1 = obj->x48 - obj->x4C;
        *out2 = obj->x44;
        *out3 = value - (obj->x48 - obj->x4C);
    }
}
