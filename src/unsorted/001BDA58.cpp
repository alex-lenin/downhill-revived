/* UNIT 0x001BDA58 0x001BDDA0 .rodata=0x28F530:0x28F5A0 */
#include "skat_queue.h"
#include "event_queue.h"
struct State { int x0,x4,x8,xC,x10; };
struct Base { virtual void v0(); virtual void v1(); virtual void v2(); virtual char *v3(); };
struct Obj: Base { unsigned int **x4; };
struct Global { char pad[0x5350]; int x5350; int x5354; char rest[0x1000]; void *info() { return &x5350; } void *queue() { return &x5354; } void *entry(int i) { return (char *)this+i*0x84+0x5964; } };
extern Global D_00398C40;
extern unsigned int D_004B2840[];
extern "C" {
extern int func_00238F48();
extern int func_001BC618(void *,int);

extern int func_001BC6A0(void *,int,int);
extern void func_001DB6B0(void *,int,int,int *,int *,int);
extern int func_001C0180(void *,unsigned int);
extern void func_001BC868(void *,int,int,int,int,int,int,void *,int,int,int);
extern int func_001BC480(void *,int);
extern void func_001BBF30(void *,int,void *);
}
int func_001BDA58(State *s,Obj *o,int a,int b) {
 int pair1[4],pair2[4],found=0,other,result,key,code,temp; int *pair;
 if((float)(func_00238F48()-s->x10)<15.0f) return -1;
 {
 temp=func_001BC618(D_00398C40.info(),s->x8);
 if(temp!=5) func_00235798((SkatQueueOwner *)(D_00398C40.queue()),temp);
 result=func_001BC6A0(D_00398C40.info(),s->x4,s->x0);
 if(result!=5) {
 func_001DB6B0(o->v3()+16,a,b,&found,&other,0);
 if(!found) return -1;
 {
 key=func_001C0180(D_00398C40.info(),**o->x4>>18);
 { void *data=o->v3()+16;
 func_001BC868(D_00398C40.info(),result,a,b,s->x4,0,1,data,0,0,2); }
 s->x0=func_001BC480(D_00398C40.info(),result);
 switch(D_004B2840[0]) {
 case 0: case 6: case 24:
  pair1[0]=key; pair1[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 66, (int *)(pair1), 8, -1); break;
 case 11: case 15: case 16:
  pair1[0]=key; pair1[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 67, (int *)(pair1), 8, -1); break;
 case 19: case 25:
  pair1[0]=key; pair1[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 68, (int *)(pair1), 8, -1); break;
 case 17: case 18: case 20:
  pair2[0]=key; pair2[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 69, (int *)(pair2), 8, -1); break;
 case 12: case 21: case 26:
  pair2[0]=key; pair2[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 70, (int *)(pair2), 8, -1); break;
 case 5: case 14: case 22:
  pair2[0]=key; pair2[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 71, (int *)(pair2), 8, -1); break;
 case 2: case 8: case 10:
  pair2[0]=key; pair2[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 72, (int *)(pair2), 8, -1); break;
 case 3: case 4: case 9:
  pair2[0]=key; pair2[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 65, (int *)(pair2), 8, -1); break;
 case 1: case 7: case 13:
  pair2[0]=key; pair2[1]=s->xC;
  func_00233BA8((EventQueue *)(D_00398C40.queue()), result, 73, (int *)(pair2), 8, -1); goto done;
 default: goto done;
 }
done:
 s->x10=func_00238F48();
 func_001BBF30(D_00398C40.queue(),result,D_00398C40.entry(result));
 }
 }
 return result;
 }
}
