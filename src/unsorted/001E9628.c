/* UNIT 0x001E9628 0x001E9998 */
#include "rider.h"
struct Entry { char p0[12]; int xC,x10; char p14[12]; float x20; char p24[8]; float x2C,x30,x34,x38; char p3C[24]; };
struct Object { char p0[0x440]; unsigned long x440; };
struct Anim { Rider *x0; char p4[4]; unsigned char x8; char p9[0x1F]; int x28,x2C; char p30[4]; int x34; char p38[0x194]; int x1CC; char p1D0[16]; float x1E0,x1E4; char p1E8[8]; unsigned char x1F0; char p1F1[4]; unsigned char x1F5; char p1F6[2]; AnimationHandle x1F8; char p208[16]; int x218; };
extern struct Entry D_00272888[];

extern float func_001E5198(void *);
extern void func_001E9CC0(void *,int,float);

extern void func_001AC880(void *,float);
extern float func_00236088(float,float,float,float);
extern void func_001AE210(void *,float);
extern int func_001E51A8(void);
extern int func_00138560(void *);
extern void func_001E59F8(void *);
extern void func_00218498(int,int);

extern void func_001E5990(void *,int);
void func_001E9628(struct Anim *a) {
 AnimationHandle h;
 Rider *r = a->x0;
 int old = a->x1CC;
 int index = a->x2C;
 RiderComponent *s = (RiderComponent *)((char *)r+0x1E40);
 void *control = (char *)r+0x6610;
 struct Entry *e;
 float amount, scale, factor, delta;
 AnimationHandle *hp;
 a->x1CC=0;
 if (!index) index=a->x28;
 hp=(AnimationHandle *)((char *)s+(a->x34*16+0x14));
 e=&D_00272888[index];
 h=*hp;
 amount=e->x20*0.01f;
 scale=func_00204280(&h);
 factor=func_001E5198(control);
 delta=amount*scale*factor*16.66667f;
 if(r->x78D4) ((struct Object *)r->x78D4)->x440=(((struct Object *)r->x78D4)->x440 & ~0x200000000UL)|0x200000000UL;
 a->x34=e->xC;
 if(s->x2160) {
  if(a->x8) func_001E9CC0(a,0,delta);
  if(!a->x1F0) SelectAnimationTimeFirst((AnimationHandle *)((char *)r+0x2144), delta, 1, 1);
  if(e->x30!=0.0f) func_001AC880((char *)a+0x3C,delta);
  if(e->x2C!=0.0f) func_001AC880((char *)a+0xA0,delta);
  if(e->x34!=0.0f) { a->x1E0=func_00236088(a->x1E0,0.5f,0.05f,0.01f); func_001AC880((char *)a+0x104,12.0f); }
  if(e->x38!=0.0f) { a->x1E4=func_00236088(a->x1E4,0.5f,0.05f,0.01f); func_001AC880((char *)a+0x168,12.0f); }
  if(!a->x1F0 && e->x10) { func_00202E38((AnimationHandle *)((char *)r+0x2F0), 1, 1, delta); func_001AE210((char *)r+0x48B0,delta); }
  if((func_001E51A8() || (func_00138560((char *)a->x0+0x40C0)&255)) && old!=4) func_001E59F8((char *)r+0x6610);
  if(a->x28==42) {
   func_00218498(a->x218,0);
   h=a->x1F8; func_00203E10(&h,0.0f);
   h=a->x1F8; func_00203B68(&h,0,1.0f);
   func_00202E38(&a->x1F8, 0, 1, delta*0.5f);
  }
 }
 func_001E5990(control,0);
 a->x1F5=0;
}
