/* UNIT 0x00134F30 0x00134F58 */
typedef struct Obj { char pad0[0x60]; float x60; } Obj;
extern void func_00134E88(Obj *, float);
void func_00134F30(Obj *obj, float *values) {
    obj->x60 = values[1];
    func_00134E88(obj, values[0]);
}
