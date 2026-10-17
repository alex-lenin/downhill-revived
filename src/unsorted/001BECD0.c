/* UNIT 0x001BECD0 0x001BEE08 */
#include "event_queue.h"
typedef struct { char pad[0x614]; char slots[5][0x84]; } Base;
typedef struct { char pad[0x5350]; Base x5350; } Global;
extern Global D_00398C40;
extern int D_0043F348[];
extern int func_001C0768(void *, unsigned int);
extern int func_001BC6A0(void *, int, int);
extern void func_001BC868(void *,int,int,int,int,int,int,void *,int,int,int);
extern int func_001BC480(void *,int);
extern void func_001BBF30(void *,int,void *);
typedef struct { unsigned int *x0; int x4; char pad[8]; int x10; } Obj;
int func_001BECD0(int *arg0, Obj *arg1) {
 int value, index, local; int anchor; void *base, *other;
 index = -1;
 anchor = (int)&D_00398C40;
 base = (char *)(anchor+0x5350);
 value = func_001C0768(base, *arg1->x0 >> 18);
 if (value == -1 || (unsigned int)(D_0043F348[0] - 1) >= 3) return -1;
 {
 index = func_001BC6A0(base,arg0[1],arg0[0]);
 if (index != 5) {
 other = (char *)(anchor+0x5354);
 func_001BC868(base,index,0x50,0xA0,arg0[1],0,1,&arg1->x10,0,0,0);
 arg0[0] = func_001BC480(base,index);
 local=value;
 func_00233BA8((EventQueue *)(other), index, 0x15, (int *)(&local), 4, -1);
 func_001BBF30(other,index,base + (index*0x84+0x614));
 }
 return index;
 }
}
