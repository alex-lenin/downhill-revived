/* UNIT 0x001ACCC8 0x001ACE98 */
#include "animation.h"
typedef struct Actor { char p0[0x3FA4]; int x3FA4; char p3FA8[0x39C4]; int x796C; int x7970; int x7974; char p7978[0x17C]; int x7AF4; } Actor;
typedef AnimationRange Clip;
typedef struct Table { char p0[0x10]; Clip *clips[1]; } Table;
typedef struct Resource { char p0[0x20]; Table *x20; } Resource;
typedef AnimationHandle Handle;
typedef struct Track { char p0[9]; char x9; char pA[0x5A]; } Track;
typedef struct State { int x0; char x4; char p5[3]; int x8,xC,x10,x14; char x18,x19; char p1A[2]; float x1C,x20; int x24; float x28,x2C,x30,x34,x38,x3C,x40,x44,x48; char p4C[4]; int x50; char p54[4]; Handle x58; Track x68; Handle xCC; Track xDC; Actor *x140; } State;
extern void func_001AC7B0(Track *,Handle *), func_001AC810(Track *,float);
void func_001ACCC8(State *s, Actor *a) {
 Handle temp;
 Track *track;
 s->x50=0; s->x24=0; s->x20=1.0f; s->x1C=1.0f; s->x19=0; s->x4=0;
 s->x28=1.0f; s->x30=-1.0f; s->x3C=1.8f; s->x40=5.0f;
 s->x8=0; s->xC=0; s->x10=0; s->x14=0; s->x0=0;
 s->x18=1; s->x2C=-1.0f; s->x34=1.0f; s->x44=1.8f; s->x48=5.0f;
 if (a) {
  s->x140=a;
  *(int *)((char *)&temp+8)=0;
  func_002036C8(&temp,a->x796C+16,a->x7974,47,a->x3FA4);
  func_002038E0(&temp,1);
  func_002036C8(&temp,a->x7970,a->x7974,47,s->x140->x7AF4);
  func_002038E0(&temp,1);
  func_002036C8(&s->x58,a->x7970,a->x7974,3,s->x140->x7AF4);
  func_001AC7B0(&s->x68,&s->x58);
  func_001AC810(&s->x68,0.0f);
  if (s->x58.x0) {
   Clip *clip=s->x58.x0->x20->entries[s->x58.xC];
   s->x28=clip->x8-clip->x4;
  }
  func_002036C8(&s->xCC,a->x7970,a->x7974,7,s->x140->x7AF4);
  track=&s->xDC;
  func_001AC7B0(track,&s->xCC);
  track->x9=4;
  func_001AC810(track,0.0f);
 }
}
