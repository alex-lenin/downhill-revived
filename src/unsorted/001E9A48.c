/* UNIT 0x001E9A48 0x001E9AF8 */
#include "player_slot_event.h"
struct Rider { char p0[0x10]; float x10; char p14[4]; float x18; char p1c[0x6fc]; int x718; char p71c[0x9c6]; unsigned char x10e2; char p10e3[3]; unsigned char x10e6; };
struct Obj { char pad[0x6610]; struct Rider x6610; char rest[0x291]; unsigned int x7988; };
struct Render { char pad[0x150]; char x150[0x1230]; };
extern struct { struct Render x0[4]; } D_003B2270;
extern int func_001E5E58(struct Rider*,int);

static inline void render(void *base,unsigned int index,int selected,int result,int x,int y,int field) {
 switch(index) { case 0: case 1: case 2: case 3:
  SetPlayerSlotEventLegacy((index*0x1380+0x150)+(char*)base,selected,result,x,y,field,0); break;
 }
}
void func_001E9A48(struct Obj **arg,int value) {
 struct Obj *obj=*arg;
 struct Rider *rider=&obj->x6610;
 int selected=rider->x10e2==0?value:0;
 if(rider->x10e6==0) {
  unsigned int index=*(unsigned int*)((char*)obj+0x7988);
  int field=rider->x718;
  int result=func_001E5E58(rider,selected);
  render(&D_003B2270,index,selected,result,(int)rider->x10,(int)rider->x18,field);
 }
}
