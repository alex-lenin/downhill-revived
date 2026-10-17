/* UNIT 0x00197F10 0x00197FE0 */
#include "menu_items.h"
typedef struct Obj { char pad0[0x418]; int x418; char pad41C[0x44]; int x460,x464,x468,x46C; char pad470[0x38]; int x4A8; char pad4AC[0x54]; int x500,x504,x508; } Obj;
extern int D_0029DE68;
typedef struct Table { char pad[0x20]; int x20; } Table;
extern Table D_004348F0[];
extern void func_0018E638(Obj *,int,void *);
extern void func_0018E6B8(Obj *,int,void *);
extern void func_001979B0(Obj *);
void func_00197F10(Obj *obj) {
 obj->x460=64; obj->x468=10;
 func_0018E638(obj,0x5E,&D_0029DE68);
 obj->x418=0;
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x56,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x57,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x1A,&D_0029DE68,1);
 func_0018E6B8(obj,0x10,&D_0029DE68);
 obj->x4A8=5;
 obj->x504=((Table *)((char *)D_004348F0+obj->x500*20))->x20;
 obj->x508=30;
}
