/* UNIT 0x00134F00 0x00134F30 */
typedef struct Obj { char pad0[0x60]; float x60; } Obj;
extern float func_00134E40(Obj *);
void func_00134F00(Obj *obj, float *out) {
    out[1] = obj->x60;
    out[0] = func_00134E40(obj);
}
