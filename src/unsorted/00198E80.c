/* UNIT 0x00198E80 0x00198F10 */
#include "menu_items.h"
struct Obj { int x0; char p4[0x414]; int x418; char p41c[0x44]; int x460; char p464[0x44]; int x4a8; };
extern int D_0029DE68;
extern char D_003F52D0[];
extern void func_0018E638(struct Obj *, int, void *);
extern void func_001B5AF0(void *, int, int);
void func_00198E80(struct Obj *o) {
 o->x460=0x69;
 func_0018E638(o,0x11,&D_0029DE68);
 func_001B5AF0(D_003F52D0,1,1);
 o->x0=0; o->x418=0;
 func_0018E3A8((struct SharedMenu *)(void *)(o),0x13,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(o),0x12,&D_0029DE68,1);
 o->x4a8=5;
}
