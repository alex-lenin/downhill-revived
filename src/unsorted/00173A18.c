/* UNIT 0x00173A18 0x00173CB0 */
#include "track_projection.h"
#if defined(__mips__)
#define CallTrackProjection func_00119180
#else
#define CallTrackProjection ProjectTrackDiscard
#endif
typedef struct Point { char pad[0x18]; float x18; char pad1C[0x84]; } Point;
typedef struct Rider { char pad[0x792C]; Point *x792C; } Rider;
typedef struct Mark { char pad[8]; float x8; } Mark;
typedef struct Obj { int x0,x4,state; Point *xC; int x10; Point *x14,*x18; char pad1C[0x28]; Mark *x44; int x48; Mark *x4C; char pad50[0x50]; char xA0[16]; } Obj;
extern struct Globals { char pad[0x50130]; Rider *riders[8]; } D_002AF2C0;
extern struct State { char pad[0x10C]; int x10C; char pad110[0x178]; int count; } D_004B25A0;
extern char D_0029EA20[];
extern int func_00222BD8(int,int);
#if defined(__mips__)
extern void func_00119180(void *,void *,void *,void *,int);
#endif
void func_00173A18(Obj *obj) {
 float minimum=99999.9f,maximum=-99999.9f; int i; Rider **p;
 for(i=0;i<D_004B25A0.count;i++) {
  Rider *r=D_002AF2C0.riders[i]; Point *point=r->x792C; float value=point->x18;
  if(value<minimum) { obj->x14=point; minimum=value; point=r->x792C; value=point->x18; }
  if(maximum<value) { obj->x18=point; maximum=value; }
 }
 if(D_004B25A0.x10C!=15) {
  if(obj->state==3) {
   if(obj->x44==obj->x4C) {
    if(obj->x4C->x8 < obj->xC->x18 && minimum < obj->xC->x18+600.0f) obj->state=1;
   } else {
    if(obj->xC->x18 < obj->x4C->x8 && maximum < obj->xC->x18-600.0f) obj->state=1;
   }
  } else if(obj->state==4) {
   if(maximum<obj->xC->x18-70.0f) obj->state=1;
  } else if(obj->state==5) {
   if(obj->xC->x18+600.0f<minimum && func_00222BD8(0,1)) {
    Point *point=obj->xC; obj->state=6; CallTrackProjection(D_0029EA20,(char *)point+0x10,obj->xA0,(char *)point+0xA0,1);
   }
  } else if(obj->state==6) {
   if(maximum<obj->xC->x18-70.0f) { if(obj->x4==1) obj->state=5; else obj->state=7; }
  } else if(obj->state==7) {
   if(obj->xC->x18+200.0f<minimum && func_00222BD8(0,1)) {
    Point *point=obj->xC; obj->state=6; CallTrackProjection(D_0029EA20,(char *)point+0x10,obj->xA0,(char *)point+0xA0,1);
   }
  }
 }
}
