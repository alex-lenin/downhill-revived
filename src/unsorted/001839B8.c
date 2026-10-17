/* UNIT 0x001839B8 0x00183B10 */
#include "menu_items.h"
struct Obj { char pad0[0x418]; int x418; char pad41C[0x44]; int x460; };
struct World { char pad0[0x20]; int x20; char pad24[8]; int x2C; char pad30[0x2E0]; int scores[8][8]; };
struct Rider { char pad0[0x661C];float score; };
extern struct World D_003C6930;
extern char D_0029DE68[];
struct League { char pad0[0x50130]; struct Rider *riders[8]; };
extern struct League D_002AF2C0;
extern int D_004B2828[];
extern void func_00183770(struct World*);
extern void func_0018DB08(struct Obj*);
void func_001839B8(struct Obj *obj) {
 int i;
 obj->x418=0;
 if(D_003C6930.x2C==8) {
  func_0018E3A8((struct SharedMenu *)(void *)(obj),0x94,D_0029DE68,1);
  func_0018E3A8((struct SharedMenu *)(void *)(obj),0x23,D_0029DE68,1);
 } else func_0018E3A8((struct SharedMenu *)(void *)(obj),0x93,D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x47,D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x1A,D_0029DE68,1);
 obj->x460=0xA5;
 if(D_003C6930.x20==1) {
  int count=D_004B2828[0];
  for(i=0;i<count;i++) {
   struct Rider *rider=D_002AF2C0.riders[i];
   D_003C6930.scores[i][D_003C6930.x2C]=(int)rider->score;
  }
  func_00183770(&D_003C6930);
 }
 func_0018DB08(obj);
}
