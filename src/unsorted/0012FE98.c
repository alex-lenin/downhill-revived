/* UNIT 0x0012FE98 0x00130010 .sdata=0x29C29C:0x29C2A0 */
#include "impulse.h"
typedef struct Vec {float x,y,z,w;} Vec __attribute__((aligned(16)));
typedef struct Frame {char pad[16];Vec x10;} Frame;
typedef struct Body {char pad[0x150];float x150;} Body;
typedef struct Obj {char pad[0x3FD0];char x3FD0[0xF0];Body x40C0;char pad4214[0x3718];Frame *x792C;} Obj;
typedef struct Hit {unsigned short x0,x2;} Hit;
float D_0029C29C=5000.0f;

extern void func_00155E78(char *,int);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0":"=m"(dst):"r"(src):"$2")
static inline void Sub(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0":"=m"(*out):"r"(a),"r"(b));
}
static inline void Scale(Vec *out,Vec *v,float scale) {
 __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0":"=m"(*out):"r"(v),"f"(scale):"$2");
}
static inline float Length(Vec *v) {
 float length;
 __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0":"=&f"(length):"f"(v->x),"f"(v->y),"f"(v->z));
 return length;
}
static inline void Normalize(Vec *v) {
 float length,inv,x=v->x,y=v->y,z=v->z;
 __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3":"=f"(length):"f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0, %1, %2":"=&f"(inv):"f"(1.0f),"f"(length));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
void func_0012FE98(Obj *obj,Vec *pos,Hit *hit) {
 Vec dir;
 float power,force;
 Body *body;
 switch(hit->x2) {
 case 0x500:case 0x505:
  Sub(&dir,&obj->x792C->x10,pos);
  Normalize(&dir);
  power=100.0f;
  break;
 default:case 0x501:
  COPY_VECTOR(dir,(char *)obj+0x4180);
  dir.z=Length(&dir)*0.5f;
  Normalize(&dir);
  power=500.0f;
 }
 body=&obj->x40C0;
 force=body->x150*power;
 if(D_0029C29C<force) force=D_0029C29C;
 Scale(&dir,&dir,force);
 func_00231CC0((ImpulseBody *)(body), (Vector4 *)(&dir), (Vector4 *)(&obj->x792C->x10));
 func_00155E78(obj->x3FD0,2);
}
