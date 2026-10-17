/* UNIT 0x00134DE8 0x00134E40 */
typedef struct Obj { char pad[0x60]; int x60; float x64,x68,x6C; int x70; } Obj;
extern void func_00134E88(Obj *, float);
void func_00134DE8(Obj *obj) {
 obj->x70=0; obj->x60=0;
 func_00134E88(obj,0.5f);
 obj->x64=80.0f; obj->x68=10.0f; obj->x6C=7.0f;
}
