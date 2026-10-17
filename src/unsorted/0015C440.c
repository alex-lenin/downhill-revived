/* UNIT 0x0015C440 0x0015C4A0 */
struct Owner { char pad[0x7AC0]; float x7AC0; };
struct Obj { struct Owner *x0; };
extern int func_00138560(void *);
extern float D_0029C708;
int func_0015C440(struct Obj *p) {
 int result=0;
 if ((func_00138560((char *)p->x0 + 0x40C0) & 0xFF) && D_0029C708 < p->x0->x7AC0) result=1;
 return result;
}
