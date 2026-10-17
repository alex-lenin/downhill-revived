/* UNIT 0x001C18A0 0x001C1B48 .rodata=0x28F9D0:0x28FA80 */
#include "event_queue.h"
typedef struct Obj { int x0,x4,x8; } Obj;
typedef struct Record { char pad[0x694]; } Record;
typedef struct Global { char pad[0x5350]; int manager; char payload[0x610]; char records[8][0x84]; } Global;
extern Global D_00398C40; extern char D_004B2914[];
static __inline__ void *get_manager(Global *g) { return &g->manager; }
static __inline__ void *get_payload(Global *g) { return g->payload; }
static __inline__ void *get_record(Global *g,int id) { return (char *)g+id*0x84+0x5964; }
extern Record D_004225A0[];
static __inline__ Record *get_profile(int index) { return &D_004225A0[index]; }
extern struct State { char pad[0x10C]; int x10C; char pad110[0x180]; int x290; } D_004B25A0;
extern int func_001BC6A0(void *,int,int);
extern void func_001BC868(void *,int,int,int,int,int,int,int,int,int,int);
extern int func_001BC480(void *,int);
extern void func_001C69E0(void *,int);
extern int func_00194C80(void *,int);
extern void func_001BBF30(void *,int,void *);
void func_001C18A0(Obj *obj,int type,int value) {
 int pair[4]; int p55,p2F,p53,p50,p2E; int id; void *manager=get_manager(&D_00398C40);
 id=func_001BC6A0(manager,obj->x4,obj->x0);
 if(id!=5) {
  func_001BC868(manager,id,0,0,obj->x4,0,0,0,0,0,8);
  obj->x0=func_001BC480(manager,id);
  func_001C69E0(D_004B2914,obj->x0);
  switch(type) {
  case 0x55: p55=value; func_00233BA8((EventQueue *)(get_payload(&D_00398C40)), id, 0x55, (int *)(&p55), 4, -1); break;
  case 0x2D: pair[0]=func_00194C80(get_profile((D_004B25A0.x290>>5)-1),D_004B25A0.x10C)-1; pair[1]=value; func_00233BA8((EventQueue *)(get_payload(&D_00398C40)), id, 0x2D, (int *)(pair), 8, -1); break;
  case 0x2F: p2F=value; func_00233BA8((EventQueue *)(get_payload(&D_00398C40)), id, 0x2F, (int *)(&p2F), 4, -1); break;
  case 0x30: pair[0]=func_00194C80(get_profile((D_004B25A0.x290>>5)-1),D_004B25A0.x10C)-1; pair[1]=value; func_00233BA8((EventQueue *)(get_payload(&D_00398C40)), id, 0x30, (int *)(pair), 8, -1); break;
  case 0x53: p53=value; func_00233BA8((EventQueue *)(get_payload(&D_00398C40)), id, 0x53, (int *)(&p53), 4, -1); break;
  case 0x50: p50=func_00194C80(get_profile((D_004B25A0.x290>>5)-1),D_004B25A0.x10C)-1; func_00233BA8((EventQueue *)(get_payload(&D_00398C40)), id, 0x50, (int *)(&p50), 4, -1); break;
  case 0x2E: p2E=value; func_00233BA8((EventQueue *)(get_payload(&D_00398C40)), id, 0x2E, (int *)(&p2E), 4, -1); break;
  }
  func_001BBF30(get_payload(&D_00398C40),id,get_record(&D_00398C40,id));
 }
 obj->x8=id;
}
