/* UNIT 0x001B5DE8 0x001B6488 */
#include "particle_emission.h"
#include "particle.h"
/* The vector scratch pointer is reused for the basis and effect matrices. */
struct Vec { float x,y,z,w; } __attribute__((aligned(16)));
typedef unsigned int Quad __attribute__((mode(TI)));
typedef struct Type { int x0; short x4; unsigned short x6; } Type;
typedef struct Effect {
 char pad0[0x2630]; Vec x2630; char pad2640[0x40]; Vec x2680; Vec x2690;
 char pad26A0[0x10]; Vec x26B0[4]; char pad26F0[0x10]; Vec x2700;
 char pad2710[0x10]; unsigned int x2720; short x2724; unsigned short x2726;
 int x2728; float x272C; char pad2730[0x10]; float x2740,x2744;
 char pad2748[0x10]; unsigned int x2758; char pad275C[0x10]; unsigned short x276C;
 char pad276E[0xBA]; unsigned int x2828; char pad282C[0x1C]; struct Effect **x2848;
} Effect;
typedef struct World { char pad0[0x7720]; char x7720[0x32C]; Effect *x7A4C,*x7A50; } World;
typedef struct Def {
 unsigned int x0; short x4; unsigned short x6; int x8; float xC; int x10; Vec *x14;
 int x18,x1C; float x20,x24; char pad28[0x10]; unsigned int x38; int x3C;
} Def;
extern Def D_0027BFC0[];
extern unsigned int D_0027C3B8[];
extern Type D_0038DB90[];
extern void func_002227A8(Vec *,Vec *,Vec *,Vec *);

extern Effect *func_0022DD88(int);
extern void func_0022DDD8(Effect *);
extern void func_001C7D68(void *);
static inline void Scale(Vec *dst,Vec *src,float s) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(src),"f"(s) : "$2");
}
static inline void Cross(Vec &dst,const Vec &a,const Vec &b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(&a),"r"(&b));
}
static inline void Add(Vec *dst,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}
static inline float Length(Vec *v) {
 float len;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(v->x),"f"(v->y),"f"(v->z));
 return len;
}
static inline float NormalizeLength(Vec *v) {
 float length,inv;
 __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(length) : "f"(v->x),"f"(v->y),"f"(v->z) : "$2");
 v->x*=inv; v->y*=inv; v->z*=inv;
 return length;
}
static inline void Normalize(Vec *out,Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
 out->x=x*inv; out->y=y*inv; out->z=z*inv;
}
/* VU0 vector operations and EE FPU normalization use SDK-style inline asm. */
#if defined(__mips__)
void func_001B5DE8(Vec *a,Vec *b,int type,Vec *c,Vec *up,unsigned int color,World *world,float speed)
#else
void SharedCall001B5DE8(struct Vector4 * a_view, struct Vector4 * b_view, int type, struct Vector4 * c_view, struct Vector4 * up_view, unsigned int color, struct Shared001B5DE8_World * world_view, float speed)
#endif
{
#if !defined(__mips__)
    Vec * a = (Vec *)a_view;
    Vec * b = (Vec *)b_view;
    Vec * c = (Vec *)c_view;
    Vec * up = (Vec *)up_view;
    World * world = (World *)world_view;
#endif

 Vec normal,pos,vel,scaled,basis[3],cross;
 Vec matrix[4]; Vec unit,combined;
 Vec *np=&normal,*pp=&pos,*vp=&vel,*scratch;
 float len,root,alpha,intensity,amount,base,mult;
 Effect *effect; unsigned int tint; int kind;
 normal=*a;
 pos=*b;
 *vp=*c;
 type &= 0xFFFF;
 if((unsigned int)type>=32) return;
 Scale(&scaled,vp,1.0f/60.0f);
 scratch=&basis[0];
 func_002227A8(&scaled,np,scratch,&basis[2]);
 Cross(cross,*np,*up);
 len=NormalizeLength(&cross);
 {
 Vec *dp=scratch;
 if(len>0.001f) {
  Cross(basis[1],cross,*dp);
  len=Length(&basis[1]);
 }
 __asm__ volatile("sqrt.s %0,%1" : "=f"(root) : "f"(speed));
 intensity=len*root*5.0f-0.25f;
 if(intensity<0.0f) intensity=0.0f;
 if(intensity>100.0f) intensity=100.0f;
 alpha=(float)((color&0xFF000000)>>24)*(1.0f/256.0f);
 amount=(len+2.0f*root)*(alpha*1.25f)-0.0625f;
 if(amount<0.0f) amount=0.0f;
 if(amount>2.0f*alpha) amount=2.0f*alpha;
 Normalize(&unit,&basis[0]);
 if(intensity>0.1f && (D_0038DB90[type].x6&8)) {
  effect=world->x7A50;
  if(effect && effect->x2758<D_0027C3B8[0]) { func_0022DDD8(effect); effect=0; world->x7A50=0; }
  Add(&combined,np,&unit);
  Normalize(&combined,&combined);
  tint=(((color>>2)&0x3F3F3F)+0x202020)|0x80000000;
  if(!effect) {
   scratch=matrix;
   func_002226C8((Matrix4 *)(scratch), (const Vector4 *)(&combined));
   effect=func_0022DD88(func_0022CDC0((Vector4 *)(pp), 15, (Matrix4 *)(scratch), (int)(tint), 3.0f, (Vector4 *)(dp)));
   if(!effect) goto second;
   world->x7A50=effect; effect->x2848=&world->x7A50;
  }
  if(effect->x276C==15) {
   effect->x2740=D_0027BFC0[15].x20*intensity;
   effect->x2744=D_0027BFC0[15].x24*intensity;
   effect->x272C=D_0027BFC0[15].xC*intensity;
   effect->x2726=D_0027BFC0[15].x6;
   effect->x2720=D_0027BFC0[15].x0|0x200000;
   effect->x2700=basis[0];
   effect->x2828=tint;
   Add(&effect->x2690,pp,D_0027BFC0[15].x14);
   func_002226C8((Matrix4 *)(effect->x26B0), (const Vector4 *)(&combined));
   effect->x2630=effect->x2690;
   effect->x2680=effect->x2690;
   func_001C7D68(world->x7720);
  }
 }
second:
 if(amount>0.1f) {
  kind=-1; if(D_0038DB90[type].x6&1) kind=14;
  base=1.0f; mult=1.5f;
  switch(D_0038DB90[type].x4) {
   case 8: mult=3.0f; kind=26; break;
   case 2: kind=29; base=0.5f; mult=2.0f; color=0x801476E1; break;
   case 30: base=0.5f; mult=2.0f; kind=28; break;
   case 32: mult=2.0f; kind=33; break;
  }
  if(kind==-1) return;
  effect=world->x7A4C;
  if(effect && effect->x2758<D_0027BFC0[kind].x38) { func_0022DDD8(effect); effect=0; world->x7A4C=0; }
  if(!effect) {
   scratch=matrix;
   func_002226C8((Matrix4 *)(scratch), (const Vector4 *)(np));
   effect=func_0022DD88(func_0022CDC0((Vector4 *)(pp), kind, (Matrix4 *)(scratch), (int)(color), base+amount*mult, (Vector4 *)(dp)));
   if(!effect) return;
   world->x7A4C=effect; effect->x2848=&world->x7A4C;
  }
  if(effect->x276C==kind) {
   effect->x2740=D_0027BFC0[kind].x20*(amount+0.5f);
   effect->x2744=D_0027BFC0[kind].x24*(amount+0.5f);
   effect->x2726=D_0027BFC0[kind].x6;
   effect->x2720=D_0027BFC0[kind].x0|0x200000;
   effect->x2700=basis[0];
   Add(&effect->x2690,pp,D_0027BFC0[kind].x14);
   func_002226C8((Matrix4 *)(effect->x26B0), (const Vector4 *)(np));
   effect->x2630=effect->x2690;
   effect->x2680=effect->x2690;
  }
 }
 }
}
