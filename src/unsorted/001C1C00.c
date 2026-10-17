/* UNIT 0x001C1C00 0x001C1F48 .rodata=0x28FAC0:0x28FB60 */
#include "event_queue.h"
struct Entry { char x0[0x84]; };
struct Manager { int x0; char x4[0x610]; struct Entry entries[4]; };
struct Global { char p0[0x5350]; struct Manager mgr; };
struct Game { char p0[0x10C]; int x10C; char p110[0x180]; int x290; };
struct Item { char x0[0x694]; };
struct Course { struct Item items[44]; char pad[0x12218-44*0x694]; short x12218[256]; };
extern struct Global D_00398C40;
extern struct Game D_004B25A0;
extern struct Course D_004225A0;
static inline struct Course *course(void) { return &D_004225A0; }
static inline struct Item *item(int i) { return &course()->items[i]; }
static inline struct Global *global(void) { return &D_00398C40; }
static inline struct Entry *entry(int i) { char *p=(char *)global()+i*0x84; return (struct Entry *)(p+0x5964); }
extern int func_001BC6A0(void *,int,int);
extern void func_001BC868(void *,int,int,int,int,int,int,int,int,int,int);
extern int func_001BC480(void *,int);
extern int func_001C0958(void *);
extern int func_001C0A68(void *);
extern int func_00194C80(void *,int);
extern int func_001BFFF0(void *,int);
extern void func_001BBF30(void *,int,void *);
void func_001C1C00(int *obj,int id) {
 struct Manager *mgr=&global()->mgr;
 int slot;
 int args[4];
 int value;
 slot=func_001BC6A0(mgr,obj[1],obj[0]);
 if(slot!=5) {
  func_001BC868(mgr,slot,0,0,obj[1],0,0,0,0,0,8);
  obj[0]=func_001BC480(mgr,slot);
  switch(id) {
  case 44:
   args[0]=func_001C0958(&global()->mgr);
   args[1]=func_001C0A68(&global()->mgr);
   args[2]=func_00194C80(item((D_004B25A0.x290>>5)-1),D_004B25A0.x10C);
   args[3]=func_001BFFF0(&global()->mgr,course()->x12218[D_004B25A0.x10C]);
   func_00233BA8((EventQueue *)(global()->mgr.x4), slot, 44, (int *)(args), 16, -1);
   break;
  case 49:
   args[0]=func_001C0958(&global()->mgr);
   args[1]=func_001C0A68(&global()->mgr);
   args[2]=func_00194C80(item((D_004B25A0.x290>>5)-1),D_004B25A0.x10C);
   func_00233BA8((EventQueue *)(global()->mgr.x4), slot, 49, (int *)(args), 12, -1);
   break;
  case 79:
   value=func_00194C80(item((D_004B25A0.x290>>5)-1),D_004B25A0.x10C);
   func_00233BA8((EventQueue *)(global()->mgr.x4), slot, 79, (int *)(&value), 4, -1);
   break;
  case 50:
   args[0]=func_001C0958(&global()->mgr);
   args[1]=func_001C0A68(&global()->mgr);
   func_00233BA8((EventQueue *)(global()->mgr.x4), slot, 50, (int *)(args), 8, -1);
   break;
  case 82:
   args[0]=func_001C0958(&global()->mgr);
   args[1]=func_001C0A68(&global()->mgr);
   args[2]=func_00194C80(item((D_004B25A0.x290>>5)-1),D_004B25A0.x10C);
   args[3]=func_001BFFF0(&global()->mgr,course()->x12218[D_004B25A0.x10C]);
   func_00233BA8((EventQueue *)(global()->mgr.x4), slot, 82, (int *)(args), 16, -1);
   break;
  }
  func_001BBF30(global()->mgr.x4,slot,entry(slot));
 }
 obj[2]=slot;
}
