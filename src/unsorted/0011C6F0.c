/* UNIT 0x0011C6F0 0x0011C8F0 */
#include "timer.h"
typedef struct Settings { float x0,x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28,x2C; } Settings;
typedef struct Rider { char pad[0xB8]; float xB8; } Rider;
extern char D_00759780[], D_00260898[];
extern int func_0023D608(void);
extern void func_00167750(Settings *, int, int);
extern void func_00238BC8(int);
extern void func_0023D1A0(int);
extern Rider *func_0023D040(int);
static __inline__ void Init(Settings *p, float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, float a11) {
 p->x0=a0;
 p->x4=a1;
 p->x8=a2;
 p->x10=a4;
 p->x14=a5;
 p->x18=a6;
 p->x1C=a7;
 p->x20=a8;
 p->x2C=a11;
 p->x24=a9;
 p->xC=a3;
 p->x28=a10;
}
void func_0011C6F0(Settings *p) {
 int i=0; Rider *r;
 Init(p,900,16380,16380,1,128,128,128,245,7000,-5980,1,1);
 Init(p+1,80,1000,8000,.25f,96,110,85,210,-7160,-7716,.25f,1);
 Init(p+2,900,16380,16380,1,110,110,110,255,-8530,-11500,1,1);
 func_00167750(p,3,func_0023D608());
 func_00238BC8(0);
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(D_00260898), p, 1);
 for(;i<func_0023D608();i++) {
  func_0023D1A0(i);
  r=func_0023D040(i);
  if(r) r->xB8=1.0f;
 }
}
