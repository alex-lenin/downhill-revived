/* UNIT 0x0015C850 0x0015C920 */
class State {
public:
 char *base;
 virtual void unk1();
 virtual void unk2();
 virtual int active();
 void func_0015C850();
};
extern "C" void func_0015BFF8(void *,State *);
void State::func_0015C850() {
 char *obj=base; void *dst=obj+0x63dc; State *state;
 if (!active()) {
  state=(State *)(obj+0x63e0);
  if(state->active()) func_0015BFF8(dst,state);
  else {
   state=(State *)(obj+0x6400);
   if(state->active()) func_0015BFF8(dst,state);
   state=(State *)(obj+0x63e8);
   if(state->active()) func_0015BFF8(dst,state);
  }
 }
}
