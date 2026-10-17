/* UNIT 0x00127098 0x00127170 */
struct Flags { unsigned long x0; };
struct Base {
 virtual void v1();
 virtual void v2();
 virtual void v3();
 virtual Flags *v4();
};
struct Asset : Base {
 void *x4;
 char *x8;
};
struct Obj { int x0,x4; char pad8[0x128]; Asset *x130; int x134; char pad138[0x2C]; int x164; };
extern "C" void func_00126B20(Obj *, int);
extern "C" void func_001AC880(void *, float);
extern "C" void func_002182A8(void *, int);
extern "C" void func_00166948(int);
void func_00127098(Obj *obj) {
 Flags *flags;
 char *other;
 obj->x0 = 1;
 obj->x4 = 0;
 if (obj->x164) obj->x164 = 0;
 func_00126B20(obj, 0);
 func_001AC880((char *)obj + 0x68, 0.0f);
 flags = obj->x130->v4();
 other = obj->x130->x8;
 ((char *)flags)[4] = 0;
 if (other) other[4] = 0;
 flags->x0 &= 0xFDFFFFFFFFFFFFFFUL;
 if (obj->x134) {
  if (obj->x130->x8) func_002182A8(obj->x130->x8, 1);
  func_00166948(obj->x134);
  obj->x134 = 0;
 }
}
