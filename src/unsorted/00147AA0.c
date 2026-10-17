/* UNIT 0x00147AA0 0x00147BC8 .sdata=0x29C5A8:0x29C5B0 */
#include "timer.h"
struct Obj {int x0,x4,x8,xC;float x10,x14,x18,x1C;int x20;};
int D_0029C5A8=0;
int D_0029C5AC=0;
extern struct Obj D_00373820[];
extern char D_00759780[],D_00261940[];
struct Obj *func_00147AA0(void) {
 struct Obj *p=0;
 if(D_0029C5A8<15) {
 p=&D_00373820[D_0029C5A8];
 p->x0=0;p->x4=0;p->x8=0;p->x10=.01f;p->x14=10.f;p->x18=3.f;p->x1C=.3f;
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(D_00261940), p, 1);D_0029C5A8++;
 }return p;
}

void func_00147B50(void) { D_0029C5A8 = 0; D_0029C5AC = 0; }
extern char D_00373A60[];
extern int func_00147D90(void *, int, void *, void *, struct Obj *, int);
void func_00147B60(struct Obj *obj, int arg1, void *arg2, void *arg3) {
    if (D_0029C5AC < 255) {
        obj->x4 = func_00147D90(D_00373A60 + D_0029C5AC * 0xB0, arg1, arg2, arg3, obj, obj->x4);
        D_0029C5AC++;
        obj->x0++;
    }
}
