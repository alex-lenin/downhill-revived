/* UNIT 0x0016E5D8 0x0016E678 */
struct Flags { unsigned long bits; };
struct Child { virtual void a(); virtual void b(); virtual void c(); virtual Flags *get(); };
struct Obj { char pad0[8]; int x8; char padC[0x124]; Child *x130; };
extern void func_00126B20(Obj *, int);
extern void func_0016F968(void *, Obj *, float);
void func_0016E5D8(char *self, Obj *obj, float value) {
 if (obj->x8 != 8) {
  obj->x130->get()->bits &= 0xFDFFFFFFFFFFFFFFUL;
  func_00126B20(obj, 8);
  func_0016F968(self + 0x10, obj, value);
 }
}
