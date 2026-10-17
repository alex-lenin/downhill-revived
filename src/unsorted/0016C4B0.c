/* UNIT 0x0016C4B0 0x0016C6F8 */
#include "timer.h"
typedef struct Row { float x0,x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28,x2C; } Row;
/* func_00167750 is void in its own TU, but this caller must see an int-valued call:
   retail's $a1/$a2 setup order only reproduces when the call sets $v0 (stale or
   implicit declaration in the original TU); the result is ignored. */
extern int func_00167750(Row *, int, int);
extern int func_0023D608(void);
extern void func_00238BC8(int);
extern char D_00759780[];
extern void func_00261CD0(void);
void func_0016C4B0(Row *obj) {
 Row *a=&obj[0], *b=&obj[1], *c=&obj[2], *d=&obj[3];
 a->x0=200.0f;
 a->x4=1600.0f;
 a->x8=1200.0f;
 a->x10=100.0f;
 a->x14=100.0f;
 a->xC=0.5f;
 a->x18=75.0f;
 a->x1C=225.0f;
 a->x2C=6.0f;
 a->x20=5500.0f;
 a->x24=4900.0f;
 a->x28=0.5f;
 b->x0=700.0f;
 b->x4=6000.0f;
 b->x8=13700.0f;
 b->x10=15.0f;
 b->x14=15.0f;
 b->x18=26.0f;
 b->x1C=255.0f;
 b->x20=4200.0f;
 b->x24=-6000.0f;
 b->x2C=1.0f;
 b->xC=1.0f;
 b->x28=1.0f;
 c->x0=300.0f;
 c->x4=2000.0f;
 c->x8=1800.0f;
 c->x10=103.0f;
 c->x14=112.0f;
 c->x18=77.0f;
 c->x1C=240.0f;
 c->x2C=5.0f;
 c->x20=-8600.0f;
 c->x24=-9800.0f;
 c->xC=0.5f;
 c->x28=0.5f;
 d->x0=700.0f;
 d->x4=6000.0f;
 d->x8=13700.0f;
 d->x10=15.0f;
 d->x14=15.0f;
 d->x18=26.0f;
 d->x1C=255.0f;
 d->x20=-10800.0f;
 d->x24=-11300.0f;
 d->x2C=1.0f;
 d->xC=1.0f;
 d->x28=1.0f;
 func_00167750(a,4,func_0023D608());
 func_00238BC8(0);
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00261CD0), obj, 1);
}
