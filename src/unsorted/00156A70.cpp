/* UNIT 0x00156A70 0x001572B0 */
#include "track_projection.h"
#if defined(__mips__)
#define CallTrackProjection func_00119180
#else
#define CallTrackProjection ProjectTrackDiscard
#endif
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundHeightFirst
#endif
/* C++ (the Entry::state() call keeps the two idx*0x380 multiplies apart, as in retail).
   The spill-slot order of the hoisted pointers (matrix rows, normal, obj->x50) depends on the
   GCSE hash table size, i.e. on the RTL insn count: the bool flags and the two CROSS
   statements are what give retail's order. VU0/FPU-extra ops use inline asm helpers. */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef float Vec[4] __attribute__((aligned(16)));
typedef struct Matrix { Vec rows[4]; } Matrix;
typedef struct Body { char pad0[8]; float x8; char padC[4]; Vec pos; char pad20[0x20]; Matrix mat; } Body;
typedef struct Sub { char pad0[0x34]; void *x34; void *x38; } Sub;
typedef struct Spring { char pad0[0xC]; float xC; } Spring;
typedef struct Rider {
 char pad0[0x1E40]; char x1E40[0x16D4]; Body *x3514;
 char pad3518[0x40C0-0x3518]; Sub x40C0;
 char pad40FC[0x4220-0x40FC]; char x4220[0x43A0-0x4220];
 Spring x43A0; char pad43B0[0x4420-0x43B0]; Spring x4420;
 char pad4430[0x6610-0x4430]; char x6610[0x78D4-0x6610]; void *x78D4;
 char pad78D8[0x792C-0x78D8]; Body *x792C;
 char pad7930[0x797C-0x7930]; int x797C; int x7980; int x7984; unsigned int x7988;
 char pad798C[0x7A5C-0x798C]; int x7A5C; char pad7A60[8]; unsigned char x7A68;
 char pad7A69[0x7BD0-0x7A69];
} Rider;
typedef struct Obj { Rider *x0; char pad4[0x20-4]; Vec x20,x30,x40,x50; char pad60[0xC8-0x60]; unsigned long xC8; } Obj;
typedef struct Global { char pad0[0x10C]; int x10C; char pad110[0x288-0x110]; int x288; char pad28C[0x2A0-0x28C]; int x2A0; } Global;
typedef struct Riders { char pad0[0x1A8]; int count; } Riders;
typedef struct Entry { char pad0[0x2B8]; int x2B8; char pad2BC[0x380-0x2BC]; int state() const { return x2B8; } } Entry;
extern char D_0029EA20[],D_0032FC60[];
typedef struct World { Rider riders[10]; char pad[0x50130-10*0x7BD0]; Rider *active[16]; } World;
extern World D_002AF2C0;
extern Global D_004B25A0;
extern Riders D_002FF2C0;
extern Entry D_0037EA60[];
#if defined(__mips__)
extern void func_00119180(void *,float *,float *,float *,int);
#endif
extern void func_00157590(Obj *,float *,float *);
extern int func_00119790(void *,float *);
extern float *func_00118650(void *,int,int,int);
extern int func_001195A8(void *,float *);
extern int func_00118640(void *,float *);
extern void func_00119700(void *,short *,short *);
extern float *func_001177F8(void *,int),*func_00117828(void *,float *);
extern void func_00119AA0(void *,float *,short *,short *);
#if defined(__mips__)
extern float func_00211980(float *,float *,float,int);
#endif
extern int func_00211390(float *,int,int,Body **,int,float);
extern int func_001572B0(Obj *,float *,float *,float);
extern void func_00221F50(Matrix *,float *),func_00231A78(Sub *,Matrix *),func_00231A08(Sub *,float *);
extern void func_00134E88(Spring *,float),func_00134D70(Spring *,float *,Sub *);
extern void func_00231A48(Sub *,float *),func_00231BD8(Sub *),func_00169F48(void *);
extern void func_0010AFE0(void *,void *,Matrix *,int),func_00135100(Sub *),func_00222460(void *,float *);
extern void func_00139660(void *,Rider *),func_001E67A8(void *),func_0014BE70(Entry *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(dst):"r"(src):"$2","memory")
#define VEC_OP(dst,a,b,op) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\t" op " $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(dst):"r"(a),"r"(b))
static inline int rider_flag(Rider *r) { int flag=0; if(r && r->x7A68) flag=1; return flag; }
static inline void project(Obj *obj,float *pos,float *src,float *direction) {
 bool flag=rider_flag(obj->x0);
 CallTrackProjection(D_0029EA20,pos,src,direction,flag);
}
#define CROSS(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0":"=m"(dst):"r"(a),"r"(b))
static inline void setpos(Sub *s,float *p) {func_00231A08(s,p);}
void func_00156A70(Obj *obj) {
 Matrix matrix;
 Vec pos,normal,work;
 float *posPtr;
 short indices[4];
 union { Body *bodies[16]; Vec vec; } nearby;
 Vec delta;
 char *sub;
 int tries;
 float *forward,*up;
 int i,j,k,count,ok,found;
 float height,diff,length;
 Rider *r;
 Body *body;
 Sub *transform;
 Spring *spring;
 sub=obj->x0->x1E40;
 if(obj->xC8&0x10) {
  COPY_VECTOR(pos,obj->x30);
 } else {
  float *p; bool flag;
  p=obj->x20;
  if(obj->x20[2]>obj->x40[2]) p=obj->x40;
  COPY_VECTOR(pos,p);
  flag=rider_flag(obj->x0);
  CallTrackProjection(D_0029EA20,pos,pos,obj->x50,flag);
  if(D_004B25A0.x10C==16 || D_004B25A0.x2A0==21) func_00157590(obj,pos,obj->x50);
 }
 posPtr=pos;
 if(obj->x0->x7A5C==3) {
  for(i=0;i<D_004B25A0.x288;i++) {
   Rider *r=D_002AF2C0.active[i];
   diff=r->x792C->pos[2]-pos[2];
   if(diff<30.0f && diff>-10.0f) {
    *(U128 *)normal=0;
    normal[1]=-20.0f;
    normal[2]=10.0f;
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(work):"r"((normal)),"r"(&r->x792C->mat));
    VEC_OP(pos,r->x792C->pos,work,"vadd.xyz");
    project(obj,posPtr,posPtr,obj->x50);
    if(D_004B25A0.x10C==16 || D_004B25A0.x2A0==21) func_00157590(obj,pos,obj->x50);
   }
  }
 }
 if(obj->xC8&0x80) {
  float *p;
  p=func_00118650(D_0029EA20,func_00119790(D_0029EA20,pos),0,0);
  if(func_001195A8(D_0029EA20,p)) {
   short index=func_00118640(D_0029EA20,p);
   indices[0]=index;
   indices[1]=index;
   func_00119700(D_0029EA20,&indices[0],&indices[1]);
   p=func_001177F8(D_0029EA20,indices[1]);
  }
  COPY_VECTOR(pos,p);
 }
 found=0;
 i=0;
 COPY_VECTOR(work,pos);
 __asm__ volatile("sqc2 $vf0,%0":"=m"(normal));
 height=CallGroundProbe(work,(normal),5.0f,0);
 if(height==0.0f || height>300.0f) {
  do {
   height=CallGroundProbe(work,(normal),35.0f,0);
   i++;
   if(height==0.0f || height>300.0f) work[2]+=35.0f;
   else found=1;
  } while(!found && i<5);
 } else found=1;
 if(found) pos[2]=work[2];
 else {
  float *p;
  func_00119AA0(D_0029EA20,pos,&indices[2],&indices[3]);
  if(indices[2]>=0) p=func_001177F8(D_0029EA20,indices[2]);
  else {
   pos[2]+=5.0f;
   p=func_00117828(D_0029EA20,pos);
  }
  if(p) COPY_VECTOR(pos,p);
  height=CallGroundProbe(pos,(normal),5.0f,0);
  if(height>300.0f) height=0.0f;
 }
 pos[2]-=height;
 normal[3]=0;
 tries=0;
 do {
  ok=1;
  if(tries++<5) {
   count=func_00211390(pos,0x4B0,0x1193,nearby.bodies,15,30.0f);
   for(j=0;j<count;j++) {
    body=nearby.bodies[j];
    if(body!=obj->x0->x792C && body!=*(Body **)(sub+0x16D4))
     ok=func_001572B0(obj,pos,body->pos,body->x8);
   }
   if(!ok) continue;
   count=func_00211390(pos,0x1388,0x13EC,nearby.bodies,15,30.0f);
   { int j;
   for(j=0;j<count;j++) {
    body=nearby.bodies[j];
    if(body!=obj->x0->x792C && body!=*(Body **)(sub+0x16D4)) {
     ok=func_001572B0(obj,pos,body->pos,7.0f);
     if(ok) {
      for(k=0;k<D_002FF2C0.count;k++) {
       Rider *other=&D_002AF2C0.riders[k];
       if(k!=obj->x0->x797C) {
        float *p;
        p=other->x792C->pos;
        VEC_OP(delta,p,obj->x0->x792C->pos,"vsub.xyz");
        __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0":"=&f"(length):"f"(delta[0]),"f"(delta[1]),"f"(delta[2]));
        if(length<10.0f) ok=func_001572B0(obj,pos,p,10.0f);
       }
      }
     }
    }
   }
  }
 } } while(!ok);
 up=matrix.rows[2];
 forward=matrix.rows[1];
 r=obj->x0;
 func_00221F50(&matrix,obj->x50);
 transform=&r->x40C0;
 COPY_VECTOR(matrix.rows[2],(normal));
 CROSS(matrix.rows[0],forward,up);
 CROSS(matrix.rows[1],up,matrix.rows[0]);
 func_00231A78(transform,&matrix);
 setpos(transform,pos);
 spring=&r->x4420;
 func_00134E88(spring,0.75f);
 func_00134E88(&r->x43A0,0.75f);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0":"=m"(pos):"r"(pos),"r"((normal)),"f"(spring->xC):"$2");
 func_00134D70(spring,nearby.vec,transform);
 VEC_OP(nearby.vec,pos,nearby.vec,"vsub.xyz");
 func_00231A48(transform,nearby.vec);
 func_00231BD8(transform);
 func_00169F48(r->x4220);
 if(obj->x0->x78D4) func_0010AFE0(obj->x0->x78D4,transform->x34,&matrix,1);
 func_00135100(&obj->x0->x40C0);
 func_00222460(transform->x38,obj->x50);
 obj->xC8&=~0x10UL;
 func_00139660(D_0032FC60,obj->x0);
 func_001E67A8(obj->x0->x6610);
 if(obj->x0->x7984 && D_0037EA60[obj->x0->x7988].state()!=4)
  func_0014BE70(&D_0037EA60[obj->x0->x7988]);
}
