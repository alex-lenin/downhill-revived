/* UNIT 0x00190448 0x001904B8 */
class Obj { public: char pad[0x4f8]; virtual void m1(); virtual void m2(); };
struct State { char pad[0x150]; Obj *x150; };
extern void func_00190410(State *,int);
void func_00190448(State *state, Obj *obj) {
 if (state->x150 != obj) {
  if (!obj) func_00190410(state,1);
  if (state->x150) state->x150->m2();
  state->x150 = obj;
 }
}
