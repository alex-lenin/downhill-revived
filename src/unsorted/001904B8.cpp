/* UNIT 0x001904B8 0x00190528 */
class Obj { public: int x0; void *x4; char pad8[6]; unsigned char xE, xF; virtual void m1(); };
extern void func_00190528(void *,void *,unsigned char);
void func_001904B8(void *state, Obj *obj) {
 if (obj) {
  obj->m1();
  if (obj->xF || obj->x4) func_00190528(state,obj->x4,obj->xE);
 }
}
