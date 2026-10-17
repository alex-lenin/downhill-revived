/* UNIT 0x0010BF50 0x0010C108 */
#include "vector.h"
typedef Vector4 Vec;
typedef struct Frame {char pad[16]; Vec x10; char pad20[32]; Vec x40;} Frame;
typedef struct Entity {char pad[0x792c];Frame *x792C;} Entity;
typedef struct Obj {Entity *x0,*x4,*x8;char padC[0x3f4];int x400;char pad404[16];int x414;char pad418[24];int x430;char pad434[12];unsigned long x440;} Obj;
typedef struct State {char pad0[0x1a4];int x1A4;char pad1A8[0xc4];int x26C;char pad270[0x184];unsigned char x3F4;} State;
extern int func_00238F60(void);
extern void func_001128F0(Obj *),func_001132B0(Obj *),func_0010C108(Obj *),func_0010EC80(Obj *);
extern int func_00138560(char *);
extern void func_0010C278(Obj *,State *),func_0010F068(Obj *,State *),func_0010CB70(Obj *,State *);
static inline void Sub(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*d):"r"(a),"r"(b));
}
void func_0010BF50(Obj *obj,State *state) {
 Vec delta,local;
 long flag;
 float absolute,half;
 obj->x430+=func_00238F60();
 func_001128F0(obj);func_001132B0(obj);
 if (!(func_00138560((char *)obj->x0+0x40c0)&255)) func_0010C108(obj);
 func_0010EC80(obj);
 if(!(obj->x440 & 0x100000000)) {
  int one=1;
  if(state->x3F4) {state->x26C=one;return;}
  state->x1A4=one;state->x26C=0;return;
 } else {
  if(obj->x8 && obj->x414>=301) {
   flag=0;
   Sub(&delta,&obj->x8->x792C->x10,&obj->x0->x792C->x10);
   func_00221888(&obj->x0->x792C->x40,&delta,&local);
   {
    float ly=local.y;
    if(ly>0.0f) {
     float lx=local.x;
     absolute=__builtin_fabsf(lx);
     __asm__ volatile("Lbf50_a:" : : "f"(lx),"f"(lx));
     if(ly*0.5f>absolute) flag=1;
    }
   }
   obj->x400=0;obj->x4=obj->x8;
   obj->x440=(obj->x440 & ~0x10000000000) | (flag<<40);
  }
  if(obj->x440 & 0x10000000000) {func_0010C278(obj,state);return;}
  func_0010F068(obj,state);func_0010CB70(obj,state);return;
 }
}
