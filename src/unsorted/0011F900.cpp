/* UNIT 0x0011F900 0x0011FAA0 */
struct Handle {
 int x0,x4,x8,xC;
 void func_00203DC8(int);
 void func_00203E10(float);
 void func_00202E38(int,int,float);
};
struct Obj {
 char pad0[0x10]; unsigned int **x10; char pad14[0x58]; Handle x6C;
 char pad7C[0xE4]; char x160[0xAC]; float x20C,x210;
 char pad214[0x14]; float x228; char pad22C[0x3C]; unsigned long x268;
 Handle handle() { return x6C; }
};
extern void func_001220B0(Obj *,float);
extern float func_00238F40(void);
extern void func_00236558(void *,void *,float);
extern void func_0011EDA8(Obj *);
extern void func_00120CE8(Obj *);
extern void func_0011E568(Obj *,int);
void func_0011F900(Obj *obj) {
 union { float zero[4]; Handle sound; } tmp __attribute__((aligned(16)));
 float t;
 unsigned int flags;
 if(obj->x210>0.0f) {
  __asm__ volatile("max.s %0,%1,%2" : "=&f"(t) : "f"(obj->x210-0.0073333336f),"f"(0.0f));
  func_001220B0(obj,t);
  __asm__ volatile("sqc2 $vf0,%0" : "=m"(tmp.zero));
  func_00236558(obj->x160,tmp.zero,func_00238F40());
  t/=obj->x20C;
  obj->x268 &= 0xFFFBFFFFFFFFFFFFUL;
  obj->handle().func_00203E10(obj->x228*t);
  func_0011EDA8(obj); func_00120CE8(obj);
 } else {
  flags=**obj->x10;
  if((flags>>18)-0x4BF<5U || (flags&0xFFFC0000)==0x12E00000 || (flags&0xFFFC0000)==0x134C0000) func_0011E568(obj,4);
  else {
   tmp.sound=obj->x6C;
   tmp.sound.func_00203DC8(0);
   obj->x6C.func_00202E38(1,1,600.0f);
   func_0011E568(obj,1);
  }
 }
}
