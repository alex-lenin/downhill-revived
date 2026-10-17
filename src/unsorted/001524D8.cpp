/* UNIT 0x001524D8 0x001525B8 */
struct Child {
 int x0;
 virtual void a(); virtual void b(); virtual void c(); virtual int test();
 virtual void e(); virtual void f(); virtual void g(); virtual void h(); virtual void i(); virtual void j(); virtual int get();
};
struct Obj { char *x0; char pad4[0x10]; int x14; };
struct Arg { char pad0[8]; Child *x8; };
extern void func_0012FE38(void *, void *);
void func_001524D8(Obj *obj, Arg *arg) {
 char *base = obj->x0;
 Child *child = (Child *)(base + 0x5E40);
 if (child->test()) goto found;
 child = (Child *)(base + 0x62E0);
 if (child->test()) goto found;
 child = (Child *)(base + 0x6310);
 if (child->test()) {
found:
  func_0012FE38(base, child);
 } else {
  obj->x14 = arg->x8->get();
  if (!obj->x14) func_0012FE38(base, base + 0x4ED0);
 }
}
