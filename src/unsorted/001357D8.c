/* UNIT 0x001357D8 0x00135958 */
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Hit { unsigned short x0,x2; char pad[44]; } Hit;
typedef struct Body { char pad[16]; Vec pos; } Body;
typedef struct Level { char pad[0x16d4]; Body *x16d4; } Level;
typedef struct Rider {
 char p0[0x1e40]; Level level;
 char p3518[0x40c0-0x3518]; char x40c0[0x4ed0-0x40c0];
 char x4ed0[0x6354-0x4ed0]; float x6354;
 char p6358[0x6380-0x6358]; int x6380;
 char p6384[0x792c-0x6384]; Body *x792c;
 char p7930[0x7a50-0x7930]; void *x7a50;
 char p7a54[0x7ba4-0x7a54]; Body *x7ba4;
} Rider;
typedef struct Obj { char p0[0x34]; Vec *x34; char p38[0x158-0x38]; Rider *x158; char p15c[0x4a8-0x15c]; unsigned short x4a8; } Obj;
extern void func_001AC578(Level *);
extern int func_00238F60(void);

extern void func_0012FE38(Rider *,void *);
extern void func_0022DDD8(void *);
extern void func_00231A08(void *,Vec *);
extern void func_00134AF8(void **,Body *,Vec *);
#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static __inline__ void addscaled(Vec *out,Vec *a,Vec *b,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b),"f"(scale) : "$2");
}
void func_001357D8(Obj *obj) {
 Vec pos; Hit hits[10]; Vec offset;
 Rider *self=obj->x158; Level *level=&self->level;
 int invalid,count,n; unsigned short *hit;
 func_001AC578(level);
 obj->x4a8+=func_00238F60();
 COPY_VECTOR(pos,obj->x34);
 invalid=0;
 n=QuerySpatialHitsCountLast((Vector4 *)(&pos), (Vector4 *)(&pos), (SpatialHit *)(hits), self->x6354, 1.0f, 1.0f, 10);
 if(n>0) {count=n; hit=&hits[0].x2; do {if(*hit!=0x1e71) invalid=1; count--;hit+=24;}while(count);}
 if(self->x6380>=1) invalid=1;
 if(invalid || !self->x7ba4) {
 self->x7ba4=0;
 func_0012FE38(self,self->x4ed0);
 func_0022DDD8(self->x7a50);self->x7a50=0;
 }else{
 Body *body;
 addscaled(&self->x792c->pos,&self->x792c->pos,&self->x7ba4->pos,1.4f);
 addscaled(&level->x16d4->pos,&level->x16d4->pos,&self->x7ba4->pos,1.4f);
 func_00231A08(self->x40c0,&self->x792c->pos);
 body=self->x792c;
 *(u128 *)&offset = *(u128 *)&self->x7ba4->pos;
 func_00134AF8(&self->x7a50,body,&offset);
 }
}
