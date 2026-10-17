/* UNIT 0x0015C648 0x0015C690 */
struct Obj { char *x0; int x4; int x8; };
extern void func_00202E38(char *, int, int, float);
extern int func_00238F48(void);
void func_0015C648(struct Obj *obj) { func_00202E38(obj->x0 + 0x90, 1, 1, 300.0f); obj->x8 = func_00238F48(); }
