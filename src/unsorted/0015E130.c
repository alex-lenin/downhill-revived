/* UNIT 0x0015E130 0x0015E1A0 */
#include "menu.h"
struct Obj { char pad[0x45C]; int x45C; char pad460[0x48]; int x4A8; char pad4AC[0x4C]; int x4F8; };

extern void func_0018E638(struct Obj *, int, void *);
extern void func_0018E6B8(struct Obj *, int, void *);
extern char D_00285040[];
extern int D_0029DE68;
struct Obj *func_0015E130(struct Obj *p) {
 func_0018D9D0((SharedMenu *)(p)); p->x4F8=(int)D_00285040;
 func_0018E638(p,0x14,&D_0029DE68);
 func_0018E6B8(p,0xA5,&D_0029DE68);
 p->x4A8=5; p->x45C=16;
 return p;
}
