/* UNIT 0x0012AD80 0x0012AF48 */
#include "rider_geometry.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct QueryFlags { unsigned int x0; union { unsigned int word; unsigned short x4; struct { unsigned int low:16,a:1,b:1,c:1,other:13; } bits; } x4; int pad8[2]; } QueryFlags;
typedef struct Hit { unsigned short x0; char pad2[14]; Vec pos,normal; } Hit;
typedef struct GroundActor { char pad0[0x130]; Vec plane; char pad140[0x3C]; int x17C; char pad180[0x1C]; int x19C; } Obj;
extern float D_0029C260[4],D_0029C264[4];
extern QueryFlags *func_00210940(QueryFlags *);
extern int func_00211BE0(Vec *,Hit *,float,float);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void Init(QueryFlags *f) { f->x0=-1; f->x4.x4=0xffff; f->x4.bits.a=0; f->x4.bits.b=0; f->x4.bits.c=0; }
static inline void Enable(QueryFlags *f,int value) { f->x0=0; f->x4.bits.a=value; }
static inline float Dot(Vec *a,Vec *b) { float out; __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(out) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return out; }
static inline void Set(Vec *v,float x,float y,float z,float w) { v->x=x; v->y=y; v->z=z; v->w=w; }
static inline float Height(Vec *v,Vec *pos) { if(v->z!=0.0f) return ((v->x*pos->x+v->y*pos->y)-v->w)/-v->z; return D_0029C264[0]; }
static inline void Query(Vec *pos,Hit *hit) { func_00211BE0(pos,hit,16.0f,-48.0f); }
float func_0012AD80(Obj *obj,void *position,int force) {
 Vec *pos=(Vec *)position;
 Hit hit; QueryFlags flags,*old; float distance=D_0029C260[0],height; Vec *plane;
 if(force || obj->x17C) {
  Init(&flags); Enable(&flags,1);
  old=func_00210940(&flags);
  Query(pos,&hit);
  func_00210940(old);
  if(hit.x0) { COPY_VECTOR(obj->plane,&hit.normal); obj->plane.w=Dot(&hit.normal,&hit.pos); distance=pos->z-hit.pos.z; }
  if(distance>48.0f) { Set(&obj->plane,0.0f,0.0f,1.0f,D_0029C264[0]); }
  obj->x19C=0;
 } else {
  plane=&obj->plane;
  height=pos->z; distance=height-Height(plane,pos); obj->x19C++;
 }
 return distance;
}
