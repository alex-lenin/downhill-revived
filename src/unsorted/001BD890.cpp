/* UNIT 0x001BD890 0x001BDA58 */
#include "skat_queue.h"
#include "event_queue.h"
class Base { public: virtual void a(); virtual void b(); virtual void c(); virtual char *get(); };
class Object : public Base { public: unsigned int **x4; };
struct Obj { int x0,x4,x8; };
struct State { char pad[0x5350]; char x5350[4]; char x5354[4]; };
extern State D_00398C40;
extern int D_004B2840[];
extern void func_001DB6B0(void *,int,int,int *,int *,int);
extern int func_001BC618(void *,int);
extern int func_001BC6A0(void *,int,int);
extern void func_001BC868(void *,int,int,int,int,int,int,void *,int,int,int);
extern int func_001BC480(void *,int);
extern void func_001BBF30(void *,int,void *);

void func_001BD890(Obj *obj,Object *object) {
 int params[3];
 int found=0,other;
 int index,current;
 int anchor;
 char *base;
 func_001DB6B0(object->get()+16,20,140,&found,&other,0);
 if((**object->x4>>18)==0xFEB && D_004B2840[0]==11) {
  anchor=(int)&D_00398C40;
  base=(char *)(anchor+0x5350);
  index=func_001BC618(base,obj->x0);
  if(index==5) {
   current=func_001BC618(base,obj->x8);
   if(found>0) {
    if(current==index) {
     index=func_001BC6A0(base,obj->x4,obj->x8);
     if(index!=current) {
      char *dest=(char *)(anchor+0x5354);
      func_001BC868(base,index,20,140,obj->x4,0,1,object->get()+16,0,0,2);
      obj->x8=func_001BC480(base,index);
      params[1]=1;
      func_00233BA8((EventQueue *)(dest), index, 28, (int *)(params), 12, -1);
      func_001BBF30(dest,index,base+(index*132+0x614));
     }
    }
   } else if(current!=index) {
    func_00235798((SkatQueueOwner *)((char *)(anchor+0x5354)),current);
   }
  }
 }
}
