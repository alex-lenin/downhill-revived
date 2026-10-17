/* UNIT 0x00196A20 0x00196BB0 */
#include "menu_items.h"
struct Obj { int x0; char pad4[0x414]; int x418; char pad41C[0x44]; int x460,x464,x468,x46C; char pad470[0x94]; int x504,x508; };
extern int D_0029DE68;
extern char D_003F52D0[], D_00437340[];
extern int D_004348F0[], D_0043F348[];
extern void func_0018E638(struct Obj *,int,void *,int);
extern int func_00196198(void *);
extern void func_00190660(void *,void *);
void func_00196A20(struct Obj *p) {
 int a,b;
 p->x0=0; p->x460=64; p->x468=10; p->x464=70; p->x46C=1;
 func_0018E638(p,94,&D_0029DE68,1);
 p->x418=0;
 func_0018E3A8((struct SharedMenu *)(void *)(p),25,&D_0029DE68,1);
 b=D_004348F0[0xC00/4]; a=func_00196198(D_004348F0);
 if ((unsigned)(D_0043F348[0]-2)<3) {
 if(b>0) { if(a>0) func_0018E3A8((struct SharedMenu *)(void *)(p),88,&D_0029DE68,1); }
 func_0018E3A8((struct SharedMenu *)(void *)(p),83,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(p),89,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(p),90,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(p),41,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(p),34,&D_0029DE68,1);
 }
 func_0018E3A8((struct SharedMenu *)(void *)(p),26,&D_0029DE68,1);
 p->x504=-1;
 if(p->x508) { p->x508=0; if(b>0 && a>0) func_00190660(D_003F52D0,D_00437340); }
}
