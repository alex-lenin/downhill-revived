/* UNIT 0x0012B188 0x0012B300 */
#include "rider_geometry.h"
typedef float Vector[4] __attribute__((aligned(16)));
struct Body { char pad0[4]; unsigned char flag; char pad5[11]; Vector pos; };
struct Settings { char pad[8]; float x8; };
struct Obj { char pad0[4]; struct Body *body,*other; char padC[0xC4]; Vector xD0,xE0,xF0,x100,x110,x120; char pad130[8]; float x138; char pad13C[0x2C]; float x168; };
extern void func_0012B448(struct Obj *);
extern float func_00238F40(void);

extern void func_0012B300(struct Obj *,float *,void *);
extern void func_0012AF48(struct Obj *,float *,float *,int);
extern void func_00211280(struct Body *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
void func_0012B188(struct Obj *obj) {
 Vector pos;
 float limit,height,delta,scale;
 float *direction=obj->xE0;
 float *xy=obj->xD0;
 struct Settings *settings=(struct Settings *)((char *)obj+0x130);
 func_0012B448(obj);
 limit=func_00238F40()*32.0f;
 scale=settings->x8*func_00238F40();
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(pos) : "r"(obj->body->pos),"r"(direction),"f"(scale) : "$2");
 pos[2]=obj->body->pos[2]+obj->x168*func_00238F40();
 height=func_0012AD80((GroundActor *)(obj), (void *)(pos), 0);
 if(obj->other) {
  obj->other->flag=1;
  COPY_VECTOR(obj->other->pos,pos);
  obj->other->pos[2]-=height;
  func_0012B300(obj,obj->xF0,settings);
 }
 __asm__ volatile("min.s %0,%1,%2" : "=&f"(delta) : "f"(height),"f"(limit) : "memory");
 pos[2]-=delta;
 obj->x168-=delta;
 __asm__ volatile("max.s %0,%1,%2" : "=&f"(obj->x168) : "f"(-117.333336f),"f"(obj->x168) : "memory");
 COPY_VECTOR(obj->body->pos,pos);
 xy[0]=obj->body->pos[0]; xy[1]=obj->body->pos[1];
 func_0012AF48(obj,obj->xF0,obj->x120,0);
 func_00211280(obj->body);
}
