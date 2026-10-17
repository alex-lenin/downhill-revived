/* UNIT 0x0018A9E0 0x0018AA90 */
#include "menu_items.h"
struct Obj { char pad[0x460]; int x460; char pad2[0x44]; int x4A8; char pad3[0xC]; char x4B8; };
extern int func_001873F8(struct Obj *);
extern void func_00187718(struct Obj *);
extern void func_0018E6B8(struct Obj *,int,void *);
extern void func_00187548(struct Obj *);
extern int D_0029DE30;
void func_0018A9E0(struct Obj *obj) {
 obj->x460=90; obj->x4B8=1;
 func_001873F8(obj);
 func_001873C0((struct SharedMenu *)(void *)(obj),67,0); func_001873C0((struct SharedMenu *)(void *)(obj),68,0); func_001873C0((struct SharedMenu *)(void *)(obj),0,0);
 func_00187718(obj);
 func_001873C0((struct SharedMenu *)(void *)(obj),30,1); func_001873C0((struct SharedMenu *)(void *)(obj),31,1);
 func_0018E6B8(obj,27,&D_0029DE30);
 obj->x4A8=5; func_00187548(obj);
}
