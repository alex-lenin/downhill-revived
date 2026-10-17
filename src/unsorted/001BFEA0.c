/* UNIT 0x001BFEA0 0x001BFFF0 */
#include "event_queue.h"
typedef struct { char pad[0x5350]; int x5350; char x5354[0x610]; char x5964[5][0x84]; } Global;
extern Global D_00398C40;
typedef struct { int x0,x4; char *x8; int xC,x10; } Obj;
extern int func_001BC6A0(void *,int,int);
extern int func_001BC480(void *,int);
extern void func_001BC868(void *,int,int,int,int,int,int,void *,int,int,int);
extern void func_001BBF30(void *,int,void *);
int func_001BFEA0(Obj *obj,int kind) {
 int value; int index; void *base=&D_00398C40; void *manager=base+0x5350;
 index=func_001BC6A0(manager,obj->x4,obj->x0);
 if(index!=5) {
 func_001BC868(manager,index,0x50,0xA0,obj->x4,0,1,*(char **)(obj->x8+0x792C)+0x10,0,0,0);
 obj->x0=func_001BC480(manager,index);
 switch(kind) {
 case 36: value=1; func_00233BA8((EventQueue *)((void *)((unsigned int)&D_00398C40+0x5354)), index, 0x15, (int *)(&value), 4, -1); break;
 case 37: value=10; func_00233BA8((EventQueue *)((void *)((unsigned int)&D_00398C40+0x5354)), index, 0x15, (int *)(&value), 4, -1); break;
 }
 { void *finalbase=&D_00398C40; func_001BBF30(finalbase+0x5354,index,index*0x84+finalbase+0x5964); }
 }
 return index;
}
