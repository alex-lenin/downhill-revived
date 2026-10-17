/* UNIT 0x001AD940 0x001ADA20 */
typedef struct { int x0; float x4, x8; int xC; float x10; char pad[0xF8-0x14]; char *xF8; } Obj;
extern int func_00138560(void *);
extern void func_001ADE98(Obj *, float);
extern void func_001AE0A8(Obj *);
extern float func_00236088(float, float, float, float);
void func_001AD940(Obj *obj) {
    obj->x4 = func_00236088(obj->x4, 0.0f, 0.02f, 0.01f);
    if (func_00138560(obj->xF8 + 0x40C0) & 0xFF) {
        func_001ADE98(obj, 1.0f);
        func_001AE0A8(obj);
    } else {
        obj->x8 = func_00236088(obj->x8, 0.0f, 0.2f, 0.1f);
        obj->x10 = func_00236088(obj->x10, 0.0f, 0.02f, 0.02f);
        func_001AE0A8(obj);
    }
}
