/* UNIT 0x00161600 0x00161698 */
#include "menu_items.h"
typedef struct { char pad[0x418]; int x418; char pad2[0x44]; int x460; } Obj;
extern char D_003F52D0[], D_0029DE68[];
extern void func_001B5AF0(void *,int,int);
extern void func_0018DB08(void *);
void func_00161600(Obj *obj) {
 obj->x418=0;
 func_001B5AF0(D_003F52D0,1,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x28,D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x23,D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x1A,D_0029DE68,1);
 obj->x460=0xA5;
 func_0018DB08(obj);
}
