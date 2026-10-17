/* UNIT 0x00128B80 0x00128DB0 */
#include "timer.h"
typedef struct Settings { float x0,x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28,x2C; } Settings;
typedef struct Rider { char pad[0xB8]; float xB8; } Rider;
extern char D_00759780[], func_00261788[];
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
void func_00128B80(Settings *p) {
 int i=0; Rider *r;
 Init(p,500,32000,2400,1,95,90,110,255,5000,-2600,1,1);
 Init(p+1,100,1000,1400,0,153,166,126,200,-4600,-6700,0,1);
 Init(p+2,400,16000,10000,1,120,125,135,255,-7200,-20000,1,1);
 func_00167750(p,3,func_0023D608());
 func_00238BC8(0);
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00261788), p, 1);
 for(;i<func_0023D608();i++) {
  func_0023D1A0(i);
  r=func_0023D040(i);
  if(r) r->xB8=1.0f;
 }
}
