/* UNIT 0x0019BF90 0x0019C008 */
typedef struct Obj { int x0, x4; float x8; } Obj;
extern float func_00236088(float, float, float, float);
extern void func_0019C1C0(Obj *);
int func_0019BF90(Obj *obj) {
 obj->x8 = func_00236088(obj->x8, 0.0f, 0.1f, 0.05f);
 func_0019C1C0(obj);
 return obj->x8 != 0.0f;
}
