/* UNIT 0x0010E6C0 0x0010E860 */
#include "vector.h"
typedef Vector4 Vec;
typedef struct Point { Vec pos; unsigned short x10,x12,x14; } Point;
typedef struct Obj { char *x0; char pad4[0x78]; Point *x7C; char pad80[0x234]; float x2B4; } Obj;
extern float func_00138580(void *,void *);
extern float func_0017C008(void);
extern int func_001177C8(void *,void *);
extern char D_0029EA20[];
static inline int track_len(Point *p) { return func_001177C8(D_0029EA20,p); }
static inline void mark(char *w) {
 int one=1;
 if(*(unsigned char *)(w+0x13DD)) { *(int *)(w+0x1270)=one; return; }
 *(int *)(w+0x1270)=0; *(int *)(w+0x11A8)=one;
}
static inline void tryact(Obj *obj, char *w) { char *p=w+0x40C0; if(!p || *(short *)(p+0x4A4)<16) mark(obj->x0); else obj->x7C=0; }
void func_0010E6C0(Obj *obj) {
 Vec delta,normal; Point *point; char *world; float x,y,z,len,inv,dot,t; Vec *axis; void *body;
 point=obj->x7C;
 if(point) {
  world=obj->x0;
  axis=(Vec *)(world+0x4180); body=world+0x40C0;
  x=axis->x; y=axis->y; z=axis->z;
  __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
  __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
  normal.x=x*inv; normal.y=y*inv; normal.z=z*inv;
  __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(delta) : "r"(point),"r"(*(char **)(world+0x792C)+0x10));
  __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(delta.x),"f"(normal.x),"f"(delta.y),"f"(normal.y),"f"(delta.z),"f"(normal.z));
  if(dot<0.0f) {
   char *p; Point *cur;
   if(!((point->x14>>4)&1)) goto fail;
   t=func_00138580(body,point)*func_0017C008();
   if((float)track_len(*(Point * volatile *)&obj->x7C)>t) goto fail;
   p=(char *)obj->x0+0x40C0;
   if(p && *(short *)(p+0x4A4)>=16) { obj->x7C=0; return; }
   mark(obj->x0);
  } else {
   float dist;
   __asm__ volatile("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0" : "=f"(dist) : "f"(delta.x),"f"(delta.y),"f"(delta.z));
   if(dist<obj->x2B4) {
    mark(world);
   }
  }
 }
 return;
fail:
 obj->x7C=0;
}
