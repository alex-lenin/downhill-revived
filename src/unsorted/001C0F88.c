/* UNIT 0x001C0F88 0x001C1170 .rodata=0x28F910:0x28F990 */
#include "event_queue.h"
typedef struct Item { char pad0[0x84]; } Item;
typedef struct Manager { int x0; char x4[0x610]; Item items[5]; } Manager;
typedef struct Global { char pad0[0x5350]; Manager mgr; } Global;
typedef struct Obj { int x0,x4,x8; } Obj;
extern Global D_00398C40;
extern int func_001BC6A0(Manager *,int,int),func_001BC480(Manager *,int),func_001C0958(Manager *);
extern void func_001BC868(Manager *,int,int,int,int,int,int,void *,int,int,int);
extern void func_001BBF30(void *,int,Item *);
static inline Global *GetGlobal(void) { return &D_00398C40; }
int func_001C0F88(Obj *obj) {
 int record[3]; char a,b,c,d; int id; Global *g; Manager *mgr;
 g=GetGlobal(); mgr=&g->mgr;
 id=func_001BC6A0(mgr,obj->x4,obj->x0);
 if(id!=5) {
 if(obj->x8==0x665)func_001BC868(mgr,id,0,0,obj->x4,0,0,0,0,0,2);
 else func_001BC868(mgr,id,0,0,obj->x4,0,0,0,0,0,0);
 obj->x0=func_001BC480(&GetGlobal()->mgr,id);
 switch(obj->x8) {
 case 0x652: func_00233BA8((EventQueue *)(GetGlobal()->mgr.x4), id, 0x29, (int *)(&a), 1, -1); break;
 case 0x669: func_00233BA8((EventQueue *)(GetGlobal()->mgr.x4), id, 0x28, (int *)(&b), 1, -1); break;
 case 0x655: func_00233BA8((EventQueue *)(GetGlobal()->mgr.x4), id, 0x27, (int *)(&c), 1, -1); break;
 case 0x670: func_00233BA8((EventQueue *)(GetGlobal()->mgr.x4), id, 0x23, (int *)(&d), 1, -1); break;
 case 0x665: record[0]=func_001C0958(&GetGlobal()->mgr); record[1]=1; record[2]=0; func_00233BA8((EventQueue *)(GetGlobal()->mgr.x4), id, 0x12, (int *)(record), 12, -1); break;
 }
 func_001BBF30(GetGlobal()->mgr.x4,id,(Item *)((char *)GetGlobal() + id*0x84 +0x5964));
 }
 return id;
}
