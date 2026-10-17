/* UNIT 0x001EA9A0 0x001EAAD0 */
#include "player_slot_event.h"
typedef struct State { char p0[0x18]; float x18; char p1c[0x6fc]; int x718; char p71c[0x9cc]; unsigned char x10e8,x10e9; char p10ea[14]; int x10f8; } State;
typedef struct Owner { char pad[0x6610]; State state; char p770c[0x27c]; unsigned int x7988; } Owner;
typedef struct Obj { Owner *x0; char pad[0x1c8]; int x1cc; } Obj;
struct Render { char pad[0x150]; char x150[0x1230]; };
extern struct { struct Render x0[4]; } D_003B2270;
extern void func_001EA218(Obj *);

static __inline__ void render(void *base,unsigned int index,int selected,int count,int x,int y,int field) {
 switch(index) { case 0: case 1: case 2: case 3:
 SetPlayerSlotEventLegacy((index*0x1380+0x150)+(char*)base,selected,count,x,y,field,0); break; }
}
static __inline__ void render_default(unsigned int index,int count,void *base,int field) {
 switch(index) { case 0: case 1: case 2: case 3:
 SetPlayerSlotEventLegacy((index*0x1380+0x150)+(char*)base,46,count,0,0,field,0); break; }
}
void func_001EA9A0(Obj *obj) {
 Owner *owner; State *state; int count,field,selected; unsigned int index;
 func_001EA218(obj);
 if(obj->x1cc==0) {
  owner=obj->x0;
  state=&owner->state;
  count=(int)state->x18;
  if(count>0) {
   field=state->x718;
   if(field<=0) {
    unsigned int default_index=owner->x7988;
    render_default(default_index,count,&D_003B2270,field);
   } else {
    selected=0;
    switch(state->x10f8) {
     case 1: selected=state->x10e8 ? 49:52; break;
     case 2: selected=state->x10e8 ? 50:53; break;
     case 3: case 4: selected=state->x10e9 ? 51:54; break;
    }
    index=owner->x7988;
    render(&D_003B2270,index,selected,count,0,0,field);
   }
  }
 }
}
