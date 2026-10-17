/* UNIT 0x00153448 0x001534A8 */
struct Obj { char pad[8]; float x8, xC, x10; char pad14[12]; char x20, x21; char pad22[6]; int x28; };
extern void func_001524B0(struct Obj *, int);
void func_00153448(struct Obj *p, int value) {
 func_001524B0(p, value);
 p->x8=1.2f; p->xC=40.0f; p->x10=100.0f;
 p->x20=1; p->x21=0; p->x28=0;
}
