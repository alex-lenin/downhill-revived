/* UNIT 0x0015D0C8 0x0015D148 */
#include "menu_items.h"
#include "menu.h"
struct Obj { char pad0[0x418]; int x418; char pad41C[0xDC]; void *x4F8; };

extern char D_00285508[]; extern int D_0029DE68;
struct Obj *func_0015D0C8(struct Obj *p) {
 func_0018D9D0((SharedMenu *)(p));
 p->x418=0; p->x4F8= &D_00285508;
 func_0018E3A8((struct SharedMenu *)(void *)(p),0x28,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(p),0x29,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(p),0x1A,&D_0029DE68,1);
 return p;
}
