/* UNIT 0x00120E70 0x00121200 */
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct QueryFlags { unsigned int x0; union { unsigned int word; unsigned short x4; struct { unsigned int low:16,a:1,b:1,c:1,other:13; } bits; } x4; int pad8[2]; } QueryFlags;
typedef struct Hit { unsigned short found; char pad2[6]; float x8; char padC[4]; Vec pos,normal; } Hit;
typedef struct Obj {
 char pad0[0x170]; Vec x170; char pad180[0x60]; Vec plane; Vec x1F0;
 char pad200[0x54]; float x254; float x258; char pad25C[0xC]; unsigned long x268;
} Obj;
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
extern QueryFlags *func_00210940(QueryFlags *);


static inline void Init(QueryFlags *f) { f->x0=-1; f->x4.x4=0xffff; f->x4.bits.a=0; f->x4.bits.b=0; f->x4.bits.c=0; }
static inline float Dot(Vec *a,Vec *b) { float out; __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(out) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return out; }
static inline float minf(float a,float b) { float r; __asm__ volatile("min.s %0,%1,%2":"=&f"(r):"f"(a),"f"(b)); return r; }
static inline void Project(Vec *dst,Vec *a,Vec *b,float f) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmsubx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b),"f"(f) : "$2"); }
static inline void AddScaled(Vec *dst,Vec *a,Vec *b,float f) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b),"f"(f) : "$2"); }
static inline void CopyXYZ(Vec *dst,Vec *src) { float w=dst->w; COPY_VECTOR(*dst,src); dst->w=w; }
/* removes the part of v going into the plane, keeping v->w */
static inline void ClipVelocity(Vec *v,Vec *normal) {
 Vec tmp;
 Project(&tmp,v,normal,minf(0.0f,Dot(normal,v)));
 CopyXYZ(v,&tmp);
}
static inline void Land(Obj *obj,Hit *hit,Vec *from,Vec *to) {
 Vec *plane=&obj->plane;
 COPY_VECTOR(obj->plane,&hit->normal);
 obj->plane.w=Dot(&hit->normal,&hit->pos);
 if(hit->pos.w>=0.0f) {
  Project(&obj->x1F0,to,plane,Dot(to,plane)-obj->plane.w-obj->x258);
 } else {
  AddScaled(&obj->x1F0,from,plane,hit->x8);
 }
 obj->x1F0.z-=obj->x254;
}
void func_00120E70(Obj *obj,Vec *pos,int force) {
 Vec from;
 unsigned long flags=obj->x268;
 if(!(flags&(1UL<<50)) && !force) return;
 if(!(flags&(1UL<<49)) && !force) {
  float d;
  COPY_VECTOR(from,pos);
  from.z+=obj->x254;
  d=Dot(&from,&obj->plane)-obj->plane.w-obj->x258;
  if(d<0.0f) {
   ClipVelocity(&obj->x170,&obj->plane);
   Project(&from,&from,&obj->plane,minf(0.0f,d));
   from.z-=obj->x254;
   COPY_VECTOR(*pos,&from);
  }
 } else {
  Vec to; Hit hits[8]; QueryFlags query,*old; Hit *hit; int count;
  COPY_VECTOR(from,&obj->x1F0);
  COPY_VECTOR(to,pos);
  from.z+=obj->x254;
  to.z+=obj->x254;
  Init(&query); query.x0=0;
  old=func_00210940(&query);
  count=QuerySpatialHitsCountLast((Vector4 *)(&from), (Vector4 *)(&to), (SpatialHit *)(hits), obj->x258, 1.0f, 1.0f, 8);
  func_00210940(old);
  hit=func_00211780(hits,count,0);
  if(hit) {
   Land(obj,hit,&from,&to);
   ClipVelocity(&obj->x170,&obj->plane);
   COPY_VECTOR(*pos,&obj->x1F0);
  } else {
   __asm__ volatile("sqc2 $vf0,%0" : "=m"(obj->plane));
  }
 }
}
