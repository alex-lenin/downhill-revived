/* UNIT 0x0015C7C0 0x0015C850 */
class Actor {
public:
 int x0;
 virtual int v1(); virtual int v2(); virtual int v3(); virtual int v4(); virtual int v5(); virtual int v6(); virtual int v7(); virtual int v8(); virtual int v9(); virtual int v10(); virtual int v11();
};
struct Part { char pad0[8]; Actor *x8; };
struct Obj { char *x0; int x4; int x8; };
extern void func_00202E38(void *, int, int, float);
extern int func_00238F48(void);
void func_0015C7C0(Obj *obj) {
 int index=24;
 if (obj->x0) {
  Part *part=(Part *)(obj->x0+0x5E90);
  if(part) { if(part->x8->v11()==25) index=25; }
 }
 func_00202E38(obj->x0+index*16,1,1,300.0f);
 obj->x8=func_00238F48();
}
