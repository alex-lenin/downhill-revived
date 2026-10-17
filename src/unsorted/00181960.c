/* UNIT 0x00181960 0x00181B68 */
#include "timer.h"
typedef struct Settings { float x0,x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28,x2C; } Settings;
extern int func_0023D608(void);
extern void func_00167750(void *,int,int);
extern void func_00238BC8(int);
extern char D_00261E80[];
extern void func_0023D1A0(int);
extern void *func_0023D040(int);
extern char D_00759780[];
void func_00181960(void *obj) {
 Settings *a=(Settings *)((char *)obj+0x10), *b=a+1, *c=a+2;
 int i=0; void *r;
 a->x0=300.0f;
 a->x4=3500.0f;
 a->x8=3500.0f;
 a->x2C=1.0f;
 a->x10=84.0f;
 a->x14=110.0f;
 a->x18=110.0f;
 a->x1C=255.0f;
 a->x20=13000.0f;
 a->x24=-300.0f;
 a->xC=1.0f;
 a->x28=1.0f;
 b->x0=200.0f;
 b->x4=900.0f;
 b->x8=600.0f;
 b->x10=84.0f;
 b->x14=110.0f;
 b->x18=110.0f;
 b->x1C=255.0f;
 b->x2C=1.0f;
 b->xC=0.5f;
 b->x20=-500.0f;
 b->x24=-2200.0f;
 b->x28=0.5f;
 c->x0=3250.0f;
 c->x4=10000.0f;
 c->x8=32768.0f;
 c->x10=84.0f;
 c->x14=110.0f;
 c->x2C=1.0f;
 c->x18=110.0f;
 c->x1C=255.0f;
 c->x20=-2900.0f;
 c->x24=-5000.0f;
 c->xC=1.0f;
 c->x28=1.0f;
 func_00167750(a,3,func_0023D608());
 func_00238BC8(0);
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)((void (*)(void))D_00261E80), obj, 1);
 for (; i<func_0023D608(); i++) {
 func_0023D1A0(i); r=func_0023D040(i); if(r) *(float *)((char *)r+0xb8)=1.0f;
 }
}
