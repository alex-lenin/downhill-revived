/* UNIT 0x0012A790 0x0012A9B8 .sdata=0x29C254:0x29C260 */
struct Vec { float x,y,z,w; } __attribute__((aligned(16)));
struct Handle { int x0,x4,x8,xC; float func_00203C28(); };
struct Body { char pad0[0x10]; Vec x10; };
struct Rider { char pad0[0x792C]; Body *x792C; };
struct Base {
 virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual Vec *GetPosition();
};
struct Obj : Base {
 char pad4[0x20]; Rider *x24; char pad28[0x34]; Handle x5C; char pad6C[0x64]; Vec xD0,xE0; char padF0[0xA4]; int x194,x198;
 Handle handle() { return x5C; }
};
float D_0029C254=4000.0f,D_0029C258=6000.0f,D_0029C25C=1.0f;
extern int D_004B2840[];
extern "C" {
void func_00129860(Obj *,int);
void func_00129FC8(Obj *);
void func_0012B188(Obj *);
void func_0012F110(Rider *,Obj *,float,float,float);
float func_002223F0(Vec *,Vec *);
void func_002361D8(Vec *,Vec *,Vec *,Vec *);
void func_00236558(Vec *,Vec *,float);
float func_00238F40();
float *func_00261020(Vec *);
}
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_0012A790(Obj *obj) {
 obj->x194=0;
 if(obj->x24) {
  if(obj->handle().func_00203C28()>=1.0f) func_00129860(obj,5);
  else if(!obj->x198) {
   Vec pos,unit;
   COPY_VECTOR(pos,&obj->x24->x792C->x10);
   Vec *u=&unit;
   pos.z=0.0f;
   COPY_VECTOR(*u,&pos);
   float *scale=func_00261020(u);
   float threshold,distance=0.0f;
   *scale=1.0f;
   func_002361D8(&unit,&obj->xD0,&obj->xE0,&pos);
   func_00236558(&obj->xD0,&pos,func_00238F40());
   if(D_004B2840[0]==3) threshold=0.45f;
   else if(D_004B2840[0]==12) { threshold=0.05f; distance=10.0f; }
   else { threshold=0.6f; distance=15.0f; }
   int flag=threshold<obj->handle().func_00203C28() && func_002223F0(&obj->x24->x792C->x10,obj->GetPosition())<distance;
   if((unsigned char)flag) {
    func_0012F110(obj->x24,obj,D_0029C254,D_0029C258,D_0029C25C);
    obj->x198=1;
   }
  }
 }
 func_0012B188(obj);
 func_00129FC8(obj);
}
