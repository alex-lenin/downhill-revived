/* UNIT 0x0015B798 0x0015B910 */
#include "rider_geometry.h"
typedef float Vector[4] __attribute__((aligned(16)));
struct Body { char pad0[4]; unsigned char flag; char pad5[11]; Vector pos; };
struct Settings { char pad[8]; float x8; };
struct Obj { char pad0[8]; struct Body *body,*other; char pad10[0x130]; Vector x140,x150,x160,x170,x180,x190; char pad1A0[8]; float x1A8; char pad1AC[0x30]; float x1DC; };
extern void func_0015BC20(struct Obj *);
extern float func_00238F40(void);

extern void func_0015BAD8(struct Obj *,float *,void *);
extern void func_0015B558(struct Obj *,float *,float *,int);
extern void func_00211280(struct Body *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
void func_0015B798(struct Obj *obj) {
 Vector pos;
 float limit,height,delta,scale;
 float *direction=obj->x150;
 float *xy=obj->x140;
 struct Settings *settings=(struct Settings *)((char *)obj+0x1A0);
 func_0015BC20(obj);
 limit=func_00238F40()*32.0f;
 scale=settings->x8*func_00238F40();
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(pos) : "r"(obj->body->pos),"r"(direction),"f"(scale) : "$2");
 pos[2]=obj->body->pos[2]+obj->x1DC*func_00238F40();
 height=func_0015B910((GroundNpc *)(obj), (void *)(pos), 0);
 if(obj->other) {
  obj->other->flag=1;
  COPY_VECTOR(obj->other->pos,pos);
  obj->other->pos[2]-=height;
  func_0015BAD8(obj,obj->x160,settings);
 }
 __asm__ volatile("min.s %0,%1,%2" : "=&f"(delta) : "f"(height),"f"(limit) : "memory");
 pos[2]-=delta;
 obj->x1DC-=delta;
 __asm__ volatile("max.s %0,%1,%2" : "=&f"(obj->x1DC) : "f"(-117.333336f),"f"(obj->x1DC) : "memory");
 COPY_VECTOR(obj->body->pos,pos);
 xy[0]=obj->body->pos[0]; xy[1]=obj->body->pos[1];
 func_0015B558(obj,obj->x160,obj->x190,0);
 func_00211280(obj->body);
}
