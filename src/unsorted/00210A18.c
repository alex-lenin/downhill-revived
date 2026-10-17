/* UNIT 0x00210A18 0x00210CA8 */
#include "spatial.h"
typedef unsigned int Quad __attribute__((mode(TI)));






int func_00214770(Vector4*,Vector4*,void*,int,SpatialHit*,int,int,float);
int func_00213C08(Vector4*,Vector4*,float,int,int,Vector4*,Vector4*,void*,SpatialHit*,int);


static inline void bounds(Vector4 *lo,Vector4 *hi,Vector4 *a,Vector4 *b) { __asm__ volatile("lqc2 $vf11,0(%2)
	lqc2 $vf12,0(%3)
	vmini.xyz $vf13,$vf11,$vf12
	vmax.xyz $vf14,$vf11,$vf12
	sqc2 $vf13,%0
	sqc2 $vf14,%1":"=m"(*lo),"=m"(*hi):"r"(a),"r"(b):"memory"); }
static inline void expand(Vector4 *dst,Vector4 *v,int sub) { if(sub) __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0":"=m"(*dst):"r"(dst),"r"(v):"memory"); else __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0":"=m"(*dst):"r"(dst),"r"(v):"memory"); }
int func_00210A18(SpatialCollider *obj,SpatialReference *trans,void *a,void *b,float radius,float x,float y,SpatialHit *hits,int capacity) {
 struct { Vector4 start,end; SpatialHit result; } local; int count; unsigned int i; void **child;
 if(!capacity) return 0;
 func_00212500(trans,a,&local.start,x); count=0;
 func_00212500(trans,b,&local.end,y);
 switch(obj->x0&63) {
 case 10:
  if(func_00214770(&local.start,&local.end,obj->x20,(short)obj->x4,&local.result,0,0,radius)) {
   hits->x0=1; count=1; hits->x6=obj->x8.index; hits->x8=*((float*)&local.result.x20); *(Quad*)&hits->x20=*(Quad*)&local.result.x10; *(Quad*)&hits->x10=*(Quad*)&local.result;
  }
  break;
 case 42:
  ((float*)&local.result.x20)[2]=radius; ((float*)&local.result.x20)[1]=radius; ((float*)&local.result.x20)[0]=radius;
  bounds((Vector4*)&local.result,&local.result.x10,&local.start,&local.end); expand(&local.result.x10,&local.result.x20,0); expand((Vector4*)&local.result,&local.result.x20,1);
  for(i=0;i<obj->x4;i++) count+=func_00213C08(&local.start,&local.end,radius,0,0,(Vector4*)&local.result,&local.result.x10,obj->xC[i],hits+count,capacity-count);

  { int n; for(n=0;n<count;n++) hits[n].x6=obj->x8.map[hits[n].x6]; }
  break;
 }
 postprocess:
 { int n; for(n=0;n<count;n++) { float t, value=hits[n].x10.w; __asm__ volatile("max.s %0,%1,%2":"=&f"(t):"f"(value),"f"(0.0f)); func_002125A8(trans,&hits[n].x10,&hits[n].x10,t); func_00212688(trans,&hits[n].x20,&hits[n].x20,t); hits[n].xC=(int)trans->actor; } }
done:
 return count;
}
