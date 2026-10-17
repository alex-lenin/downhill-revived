/* UNIT 0x00184A08 0x00184A78 */
class Obj { public: char pad[0x4f8]; virtual void m1(); virtual void m2(); virtual void m3(); virtual void m4(); virtual void m5(); };
struct State { char pad[8]; Obj *x8; unsigned char xC; };
struct Global { char pad[0xac]; unsigned char xAC; };
struct Root { char pad[0x64]; Global x64; Global *get() { return &x64; } };
extern Root D_003F52D0;
struct Result { int value; int get() { return value; } };
extern int D_003E3C3C[];
int func_00184A08(State *state) {
 Global *global; int result;
 if (state->xC) {
  global = D_003F52D0.get();
  global->xAC = 1;
  state->x8->m5();
  global->xAC = 0;
  result = D_003E3C3C[0] == 1;
 } else result = D_003E3C3C[0] == 1;
 return result;
}
