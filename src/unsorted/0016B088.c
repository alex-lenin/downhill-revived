/* UNIT 0x0016B088 0x0016B3D8 */
#include "timer.h"
typedef struct Row { float x0,x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28,x2C; } Row;
typedef struct Obj { float x0,x4; char pad8[8]; Row rows[5]; } Obj;
typedef struct Item { char pad[0xB8]; float xB8; } Item;
extern void func_00167750(Row *,int,int);
extern int func_0023D608();
extern void func_00238BC8(int);
extern char D_00759780[];
extern void func_00261CB0();
extern void func_0023D1A0(int);
extern Item *func_0023D040(int);
void func_0016B088(Obj *o) {
 Row *a=&o->rows[0], *b=&o->rows[1], *c=&o->rows[2], *d=&o->rows[3], *e=&o->rows[4];
 int i=0; Item *item;
 a->x0=500.0f; a->x4=5000.0f; a->x8=1800.0f; a->x2C=1.0f;
 a->x10=175.0f; a->x14=165.0f; a->x18=140.0f; a->x1C=220.0f;
 a->x20=13000.0f; a->x24=10900.0f; a->xC=1.0f; a->x28=1.0f;
 b->x0=40.0f; b->x4=400.0f; b->x8=700.0f; b->xC=0.0f;
 b->x10=90.0f; b->x14=100.0f; b->x18=85.0f; b->x1C=190.0f;
 b->x20=9700.0f; b->x24=8200.0f; b->x2C=1.0f; b->x28=0.25f;
 c->x0=120.0f; c->x4=500.0f; c->x8=800.0f; c->x2C=1.0f;
 c->x10=140.0f; c->x14=140.0f; c->x18=110.0f; c->x1C=210.0f;
 c->x20=7400.0f; c->x24=5300.0f; c->x28=0.5f; c->xC=0.25f;
 d->x0=160.0f; d->x4=600.0f; d->x8=900.0f; d->x10=150.0f; d->x14=145.0f; d->x18=115.0f; d->x1C=230.0f; d->x2C=1.0f; d->x20=4370.0f; d->x24=2700.0f; d->xC=0.25f; d->x28=0.5f;
 e->x0=400.0f; e->x4=6000.0f; e->x8=1200.0f; e->x2C=1.0f;
 e->x10=160.0f; e->x14=155.0f; e->x18=120.0f; e->x1C=255.0f;
 e->x20=1000.0f; e->x24=-1000.0f; e->xC=1.0f; e->x28=1.0f;
 o->x0=2275.0f; o->x4=0.375f;
 func_00167750(a,5,func_0023D608());
 func_00238BC8(0);
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00261CB0), o, 1);
 for(;i<func_0023D608();i++) {
  func_0023D1A0(i); item=func_0023D040(i);
  if(item) item->xB8=1.0f;
 }
}
