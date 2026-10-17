/* UNIT 0x00120208 0x001207F8 .sdata=0x29C16C:0x29C178 */
struct Vec { float x,y,z,w; };
struct Handle {
 int x0,x4,x8,xC;
 float func_00203C28();
 int func_00204230();
 void func_00203DC8(int);
 void func_00203E10(float);
 void func_00202E38(int,int,float);
};
struct Target { char pad0[0x10]; Vec x10; };
struct Rider { char pad0[0x792C]; Target *x792C; };
struct Obj;
struct Vtable { char pad0[0x30]; short delta; short pad32; void *(*get)(void *); };
struct Obj {
 Vtable *x0; char pad4[8]; int xC; char pad10[0x28]; Rider *x38;
 char pad3C[0x40]; Handle x7C,x8C,x9C;
 char padAC[0xB4]; Vec x160,x170,x180;
 char pad190[0xC0]; float x250; char pad254[0x14]; unsigned long x268;
 Handle first() { return x7C; }
 Handle second() { return x8C; }
 Handle third() { return x9C; }
};
float D_0029C16C=4000.0f;
float D_0029C170=6000.0f;
float D_0029C174=1.0f;
extern void func_001220B0(Obj *,float);
extern float func_002223F0(void *,void *);
extern void func_0012F110(Rider *,Obj *,float,float,float);
extern void func_0011E568(Obj *,int);
extern float *func_00261020(Vec *);
extern void func_002361D8(Vec *,Vec *,Vec *,Vec *);
extern float func_00238F40();
extern void func_00236558(Vec *,Vec *,float);
extern void func_00120CE8(Obj *);
extern void func_0011EDA8(Obj *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
#define SCALE(dst,src,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(src),"f"(s) : "$2")
static inline void setW(Vec *v,float w) { *func_00261020(v)=w; }
void func_00120208(Obj *o) {
 union { Vec vec,copy; Handle sound; } a,b,tmp;
 int flag;
 switch(o->xC) {
 case 1:
  flag=(a.sound=o->x7C,a.sound.func_00203C28())>=1.0f || (b.sound=o->x7C,b.sound.func_00204230())==0;
  if((unsigned char)flag) {
   if(o->x250<900.0f) {
    tmp.sound=o->x8C; tmp.sound.func_00203DC8(0);
    tmp.sound=o->x8C; tmp.sound.func_00203E10(1.0f);
    o->x8C.func_00202E38(1,1,600.0f);
    o->xC=2; o->x268 &= 0xFFFEFFFFFFFFFFFFUL;
   } else {
    tmp.sound=o->x9C; tmp.sound.func_00203DC8(0);
    tmp.sound=o->x9C; tmp.sound.func_00203E10(1.0f);
    o->x9C.func_00202E38(1,1,600.0f);
    o->xC=3;
   }
   SCALE(tmp.vec,&o->x180,1.4666667f);
   func_001220B0(o,0.07333333f);
  }
  break;
 case 2:
  *(short *)((char *)o+0x26A)=0;
  flag=(a.sound=o->x8C,a.sound.func_00203C28())>=1.0f || (b.sound=o->x8C,b.sound.func_00204230())==0;
  if((unsigned char)flag) {
   tmp.sound=o->x9C; tmp.sound.func_00203DC8(0);
   tmp.sound=o->x9C; tmp.sound.func_00203E10(1.0f);
   o->x9C.func_00202E38(1,1,600.0f);
   o->xC=3;
  }
  if(!(o->x268 & 0x1000000000000UL)) {
   int near=(a.sound=o->x8C,a.sound.func_00203C28())>0.5f && func_002223F0(&o->x38->x792C->x10,o->x0->get((char *)o+o->x0->delta))<12.0f;
   if((unsigned char)near) {
    func_0012F110(o->x38,o,D_0029C16C,D_0029C170,D_0029C174);
    o->x268 |= 0x1000000000000UL;
   }
  }
  break;
 case 3:
  flag=(a.sound=o->x7C,a.sound.func_00203C28())>=1.0f || (b.sound=o->x7C,b.sound.func_00204230())==0;
  if((unsigned char)flag) {
   if(o->x250>900.0f) func_0011E568(o,4);
   else func_0011E568(o,5);
  }
  break;
 }
 if(o->x38 && o->xC!=3 && !(o->x268 & 0x1000000000000UL)) {
  COPY_VECTOR(a.vec,&o->x38->x792C->x10);
  float one=1.0f;
  a.vec.z=0;
  COPY_VECTOR(b.copy,&a.vec); setW(&b.copy,one);
  func_002361D8(&b.vec,&o->x160,&o->x170,&a.vec);
  func_00236558(&o->x160,&a.vec,func_00238F40());
  o->x268 &= 0xFFFBFFFFFFFFFFFFUL;
  func_00120CE8(o);
 }
 func_0011EDA8(o);
}
