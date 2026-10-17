/* UNIT 0x00121348 0x00121530 */
#include "rider_geometry.h"
typedef struct {float f[4];} Vec __attribute__((aligned(16)));
typedef struct {unsigned short x0; char pad2[14]; Vec x10,x20;} Hit;
typedef struct {unsigned int mask;union {unsigned short low;unsigned int flags;} x4;} Query;
typedef struct GroundRider {char pad0[0x1D0];Vec x1D0;char pad1E0[16];Vec x1F0;char pad200[0x68];union {long x268;struct {int low;unsigned short x26C;} timer;};} Obj;
extern float D_0029C178[],D_0029C17C[];
extern int func_00210940(int),func_00211BE0(Vec *,Hit *,float,float),func_00238F60(void);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(dst):"r"(src):"$2")
static inline float Dot(Vec *a,Vec *b) {
 float result;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(result):"f"(a->f[0]),"f"(b->f[0]),"f"(a->f[1]),"f"(b->f[1]),"f"(a->f[2]),"f"(b->f[2]));
 return result;
}
static inline void Init(Query *q) { q->mask=0xffffffffU;q->x4.low=0xffff;q->x4.flags &= 0xfffeffff; q->x4.flags &= 0xfffdffff; q->x4.flags &= 0xfffbffff;}
static inline void SetPlane(Vec *v,float value) {v->f[0]=0;v->f[1]=0;v->f[2]=1;v->f[3]=value;}
static inline float Solve(Vec *v,Vec *p) {return (v->f[0]*p->f[0]+v->f[1]*p->f[1]-v->f[3])/-v->f[2];}
static inline float Height(Vec *v,Vec *p) {float height;float z=p->f[2];if(v->f[2]!=0) {height=Solve(v,p);} else height=D_0029C17C[0];return z-height;}

float func_00121348(Obj *obj,void *position,int force) {
 Vec *pos=(Vec *)position;
 Hit hit;Query q;int old;float distance=D_0029C178[0];
 if(force || (obj->x268 & (1L<<49))) {
  Init(&q);
  q.mask=0;q.x4.flags=(q.x4.flags & 0xfffeffff)|0x10000;
  old=func_00210940((int)&q);
  func_00211BE0(pos,&hit,16.0f,-48.0f);
  func_00210940(old);
  if(hit.x0) {
   COPY_VECTOR(obj->x1F0,&hit.x10);COPY_VECTOR(obj->x1D0,&hit.x20);
   obj->x1D0.f[3]=Dot(&hit.x20,&hit.x10);
   distance=pos->f[2]-hit.x10.f[2];
  }
  if(distance>48.0f) {
   SetPlane(&obj->x1D0,D_0029C17C[0]);
   COPY_VECTOR(obj->x1F0,pos);
  }
  obj->timer.x26C=0;
 } else {
  float height;
  height=Height(&obj->x1D0,pos);
  distance=height;
  obj->timer.x26C+=func_00238F60();
 }
 return distance;
}
