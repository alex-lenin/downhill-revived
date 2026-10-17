/* UNIT 0x0010E9B0 0x0010EC80 */
#include "vector.h"
typedef Vector4 Vec;
typedef struct Transform { char pad[16]; Vec x10; char pad20[32]; Vec x40,x50; } Transform;
typedef struct Rider { char pad0[0x1190]; int x1190,x1194; char pad1198[0xc0]; int x1258,x125c; char pad1260[0x177]; unsigned char x13d7,x13d8; char pad13d9[0x4180-0x13d9]; Vec x4180; char pad4190[0x792c-0x4190]; Transform *x792c; char pad7930[44]; float x795c; } Rider;
typedef struct State { char pad[0xcc]; int flags; } State;
typedef struct Obj { Rider *x0,*x4,*x8; char padc[0x1f0-12]; Vec x1f0; char pad200[0x78]; float x278; float x27c; char pad280[0x54]; State state; char pad3a4[0x440-0x3a4]; unsigned long flags; } Obj;
extern void func_00117308(State *,Vec *);
extern void func_0010F068(Obj *,int);
extern void func_0010F0D8(Obj *,int);
extern float fabsf(float);
static inline void Sub(Vec *out,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b)); }
static inline void Add(Vec *out,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b)); }
static inline float Dot(Vec *a,Vec *b) { float r; __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return r; }
static inline void Normalize(Vec *out, Vec *v) { float x=v->x,y=v->y,z=v->z,len,inv; __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(len) : "f"(x),"f"(y),"f"(z)); __asm__ volatile("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f),"f"(len)); out->x=x*inv; out->y=y*inv; out->z=z*inv; }
int func_0010E9B0(Obj *obj,int arg) {
 Vec delta,projection,pos,matrix[4];
 State *state;
 Rider *rider;
 Vec *axis;
 float direction,dot;
 int result=1;
 Sub(&delta,&obj->x4->x792c->x10,&obj->x0->x792c->x10);
 func_00221888(&obj->x0->x792c->x40,&delta,&projection);
 state=&obj->state;
 func_00117308(state,&obj->x1f0);
 if (((state->flags>>2)&1) || ((state->flags>>1)&1)) {
  if(obj->x8) direction=obj->x8->x795c; else direction=obj->x278;
 } else direction=obj->x278;
 if(direction>0.0f) obj->x1f0.x=-obj->x1f0.x;
 func_0010F068(obj,arg);
 rider=obj->x0; if(rider->x13d7) rider->x1258=0; else { rider->x1190=0; rider->x1258=0; }
 rider=obj->x0; if(rider->x13d8) rider->x125c=0; else { rider->x1194=0; rider->x125c=0; }
 Normalize(&delta,&obj->x4->x4180);
 func_002226C8(matrix,&delta);
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 16(%2)\n\tlqc2 $vf14, 32(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(projection) : "r"(&obj->x1f0),"r"(matrix));
 Add(&pos,&projection,&obj->x4->x792c->x10);
 Sub(&delta,&pos,&obj->x0->x792c->x10);
 Normalize(&delta,&delta);
 axis=&obj->x0->x792c->x40;
 if(Dot(&obj->x0->x792c->x50,&delta)<0.5f) result=0;
 else {
  float mag;
  dot=Dot(axis,&delta);
  obj->x27c=dot;
  /* Zero-byte labels (cf. src/unsorted/00137950.c): the first keeps the swc1 above abs.s;
     the second keeps dot live past abs.s (own register) and its two uses give dot's
     allocation priority over the 0.053f constant ($f0 vs $f1, retail). */
  __asm__ volatile("Le9b0_a:");
  mag=fabsf(dot);
  __asm__ volatile("Le9b0_b:" : : "f"(dot), "f"(dot));
  if(mag<0.053f) obj->x27c=0.0f;
 }
 if(result) { func_0010F0D8(obj,arg); obj->flags|=0x20000000000UL; }
 return result;
}
