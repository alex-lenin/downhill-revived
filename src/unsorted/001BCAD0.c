/* UNIT 0x001BCAD0 0x001BCC50 */
#include "skat_queue.h"
#include "event_queue.h"
extern char D_00398C40[];
extern int func_001BC618(void *,int);
extern int func_001BC6A0(void *,int,int);
extern void func_001DB6B0(void *,int,int,int *,int *,int);
extern void func_001BC868(void *,int,int,int,int,int,int,void *,int,int,int);
extern int func_001BC480(void *,int);
extern void func_001BBF30(void *,int,void *);

void func_001BCAD0(int *obj,int mode,void *arg2) {
 int count=0,other,property; int index,newIndex; char *game=D_00398C40; char *pool=game+0x5350; char *sub;
 index=func_001BC618(pool,obj[0]);
 if (mode==3) {
 func_001DB6B0(arg2,80,160,&count,&other,0);
 if (count>0) {
 if(index==5) {
 newIndex=func_001BC6A0(pool,obj[1],obj[0]);
 if (newIndex!=index) {
 func_001BC868(pool,newIndex,80,160,obj[1],0,1,arg2,0,0,0);
 sub=game+0x5354;
 obj[0]=func_001BC480(pool,newIndex);
 property=7;
 func_00233BA8((EventQueue *)(sub), newIndex, 33, (int *)(&property), 4, -1);
 func_001BBF30(sub,newIndex,pool+0x614+newIndex*0x84);
 }
 }
 } else if(index!=5) func_00235798((SkatQueueOwner *)(game+0x5354),index);
 } else if(index!=5) func_00235798((SkatQueueOwner *)(game+0x5354),index);
}
