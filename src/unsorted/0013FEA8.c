/* UNIT 0x0013FEA8 0x0013FF30 */
#include "menu_items.h"
typedef struct Obj { char pad[0x418]; int x418; char gap[0x8C]; int x4A8; } Obj;
extern int D_0029DD30,D_0029DE68;
extern void func_0018E638(Obj *,int,void *);
extern void func_0018E6B8(Obj *,int,void *);
extern void func_0018DC20(Obj *);
void func_0013FEA8(Obj *p) {
 p->x418=0;
 func_0018E638(p,0x3F,&D_0029DD30);
 func_0018E3A8((struct SharedMenu *)(void *)(p),0x19,&D_0029DD30,1);
 func_0018E3A8((struct SharedMenu *)(void *)(p),0x1A,&D_0029DD30,1);
 func_0018E6B8(p,0x45,&D_0029DE68);
 p->x4A8=5;
 func_0018DC20(p);
}
