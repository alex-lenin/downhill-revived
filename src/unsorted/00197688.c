/* UNIT 0x00197688 0x00197758 */
#include "menu_items.h"
typedef struct Obj { char pad0[0x418]; int x418; char pad41C[0x44]; int x460,x464,x468,x46C; char pad470[0x38]; int x4A8; char pad4AC[0x54]; int x500; } Obj;
extern int D_0029DE68;
extern void func_0018E638(Obj *,int,void *);
extern void func_0018E6B8(Obj *,int,void *);
extern void func_001979B0(Obj *);
void func_00197688(Obj *obj) {
 obj->x464=70; obj->x460=64; obj->x468=10; obj->x46C=1;
 func_0018E638(obj,0x61,&D_0029DE68);
 obj->x418=0;
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x51,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x52,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x54,&D_0029DE68,1);
 func_0018E6B8(obj,0x10,&D_0029DE68);
 obj->x4A8=5;
 if(obj->x500==-1) func_001979B0(obj);
}
