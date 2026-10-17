/* UNIT 0x0015C4A0 0x0015C4E8 */
struct Obj { char *x0; int x4; int x8; };
extern void func_00202E38(char *, int, int, float);
extern int func_00238F48(void);
void func_0015C4A0(struct Obj *obj) { func_00202E38(obj->x0 + 0xA0, 1, 1, 200.0f); obj->x8 = func_00238F48(); }
