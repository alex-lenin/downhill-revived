/* UNIT 0x0019C148 0x0019C1C0 */
typedef struct Obj { char *x0; int x4; float x8; } Obj;
extern float func_0012DEB0(void *), func_00236088(float, float, float, float);
extern void func_001B0870(void *, int), func_0019C1C0(Obj *);
void func_0019C148(Obj *obj) {
 char *owner = obj->x0;
 obj->x8 = func_00236088(obj->x8, func_0012DEB0(owner) * 0.75f, 0.4f, 0.2f);
 func_001B0870(owner + 0x4D10, 1);
 func_0019C1C0(obj);
}
