/* UNIT 0x001D8CA0 0x001D8F18 */
#include "team_roster.h"
typedef struct Child { char pad[0x20]; int flags[4]; } Child;
typedef struct Small { char pad[0x70]; } Small;
typedef struct Obj { char pad[0x10]; Child children[10]; char pad2[0x4b0-0x1f0]; int count,index; char pad3[8]; Child x4c0; Small x4f0,x560,x5d0; char x640[12]; } Obj;
typedef struct State { char pad[0x288]; int count; char pad2[0x14]; int mode; } State;
extern State D_004B25A0;

extern void func_001D2CB8(Child *);
extern void func_001D1C58(Small *);
extern void func_001DB510(void *);
extern void func_001D4E80(Obj *);
extern void func_001C0B60(void *);
void func_001D8CA0(Obj *obj) {
 int flags[4]; int i,j;
 int m;
 func_001DB9E0(0x64c,flags);
 for(i=0;i<D_004B25A0.count;i++) if(flags[i]==1) obj->children[0].flags[i]=flags[i];
 func_001DB9E0(0x645,flags);
 for(j=0;j<D_004B25A0.count;j++) if(flags[j]==1) {
   obj->children[0].flags[j]=flags[j]; obj->children[1].flags[j]=flags[j]; obj->children[2].flags[j]=flags[j];
   obj->children[4].flags[j]=flags[j]; obj->children[5].flags[j]=flags[j]; obj->children[6].flags[j]=flags[j];
   obj->children[7].flags[j]=flags[j]; obj->children[8].flags[j]=flags[j]; obj->children[9].flags[j]=flags[j];
 }
 func_001D2CB8(&obj->children[0]); func_001D2CB8(&obj->children[1]); func_001D2CB8(&obj->children[2]);
 func_001D2CB8(&obj->children[3]); func_001D2CB8(&obj->children[4]); func_001D2CB8(&obj->children[5]);
 func_001D2CB8(&obj->children[6]); func_001D2CB8(&obj->children[7]); func_001D2CB8(&obj->children[8]);
 func_001D2CB8(&obj->children[9]); func_001D2CB8(&obj->x4c0);
 func_001D1C58(&obj->x4f0); func_001D1C58(&obj->x560); func_001D1C58(&obj->x5d0);
 if((D_004B25A0.mode==12 || D_004B25A0.mode==21) && obj->count>0) {
   func_001DB510((char *)obj+0x1f0+obj->index*64);
   obj->index++;
   if(obj->index>=obj->count) obj->index=0;
 }
 m = D_004B25A0.mode;
 if(m==12) {
   func_001D4E80(obj);
   m = D_004B25A0.mode;
   if(m==12) func_001C0B60(obj->x640);
 }
}
