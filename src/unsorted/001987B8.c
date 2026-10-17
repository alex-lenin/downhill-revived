/* UNIT 0x001987B8 0x00198888 */
#include "menu_items.h"
typedef struct Obj { char pad0[0x418]; int x418; char pad41C[0x44]; int x460,x464,x468,x46C; char pad470[0x38]; int x4A8; char pad4AC[0x54]; int x500; } Obj;
extern int D_0029DE68;
extern void func_0018E638(Obj *,int,void *);
extern void func_0018E6B8(Obj *,int,void *);
extern void func_001979B0(Obj *);
extern struct Global { char pad[0x200]; } D_003F52D0;
extern int D_004B2900[];
extern void func_001B5AF0(void *,int,int);
void func_001987B8(Obj *obj) {
 obj->x460=100; obj->x468=10;
 func_001B5AF0(&D_003F52D0,1,1);
 func_0018E638(obj,0x67,&D_0029DE68);
 obj->x418=0;
 if(D_004B2900[0]) func_0018E3A8((struct SharedMenu *)(void *)(obj),0x69,&D_0029DE68,1);
 else func_0018E3A8((struct SharedMenu *)(void *)(obj),0x68,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x6A,&D_0029DE68,1);
 func_0018E6B8(obj,0xA5,&D_0029DE68);
 obj->x4A8=5;
}
