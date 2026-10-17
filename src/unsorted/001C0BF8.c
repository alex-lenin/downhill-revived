/* UNIT 0x001C0BF8 0x001C0DE0 */
#include "event_queue.h"
struct Obj { int x0,x4,x8; char pC[4]; float x10[4]; int x20,x24; };
struct Data { int x0,x4,x8; };
struct Entry { char pad[0x84]; };
struct Mgr { int x0; char x4[0x610]; struct Entry entries[1]; };
struct Global { char pad[0x5350]; struct Mgr mgr; };
extern struct Global D_00398C40;
extern int func_001BC6A0(void *,int,int);
extern void func_001BC868(void *,int,int,int,int,int,int,void *,int,int,int);
extern int func_001BC480(void *,int);
extern int func_001C0958(void *);
extern void func_001BBF30(void *,int,void *);
static inline struct Global *global(void) { return &D_00398C40; }
static inline struct Mgr *manager(struct Global *g) { return &g->mgr; }
int func_001C0BF8(struct Obj *o) {
 struct Data data;
 struct { int zero; char a,b; } extra;
 int id=func_001BC6A0(manager(global()),o->x4,o->x0);
 if(id!=5) {
  if(o->x8==18) func_001BC868(manager(global()),id,o->x20,o->x24,o->x4,0,1,o->x10,0,0,2);
  else func_001BC868(manager(global()),id,o->x20,o->x24,o->x4,0,1,o->x10,0,0,0);
  o->x0=func_001BC480(manager(global()),id);
  switch(o->x8) {
   case 20: extra.zero=0; func_00233BA8((EventQueue *)(global()->mgr.x4), id, 20, (int *)(&extra.zero), 4, -1); break;
   case 31: func_00233BA8((EventQueue *)(global()->mgr.x4), id, 31, (int *)(&extra.a), 1, -1); break;
   case 43: func_00233BA8((EventQueue *)(global()->mgr.x4), id, 43, (int *)(&extra.b), 1, -1); break;
   case 18: data.x0=func_001C0958(manager(global())); data.x4=1; data.x8=0; func_00233BA8((EventQueue *)(global()->mgr.x4), id, 18, (int *)(&data), 12, -1); break;
  }
  func_001BBF30(global()->mgr.x4,id,&manager(global())->entries[id]);
 }
 return id;
}
