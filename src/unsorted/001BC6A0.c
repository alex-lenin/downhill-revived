/* UNIT 0x001BC6A0 0x001BC810 */
#include "skat_queue.h"
struct Slot { int value; char pad[0x80]; };
struct Obj { char pad[0x628]; struct Slot slots[4]; };
extern int D_00398CD4[];
extern int func_001BC618(struct Obj*,int);
extern int func_00235590(void*,int);

int func_001BC6A0(struct Obj *obj,int value,int arg2) {
 struct Slot *base,*p,*scan;
 int result,min,index,smallest,found,i,one;
 result=func_001BC618(obj,arg2);
 if(D_00398CD4[0]) return 5;
 base=obj->slots; min=obj->slots[0].value; smallest=0; found=0; index=0; scan=base;
 do {
  int current=scan->value;
  if(current<min) { min=current; smallest=index; }
  index++; scan++;
 } while(index<4);
 if(result==5) {
  i=0; one=1; p=base;
  for(;i<4;p++,i++) {
   if(func_00235590((char*)obj+4,i)==one) {
    p->value=value; result=i; found=1; break;
   }
  }
  if(found) return result;
  p=base+smallest;
  if(p->value<value) {
   func_00235798((SkatQueueOwner *)((char*)obj+4),smallest); result=smallest;
  } else { result=5; goto end; }
 } else {
  p=base+result;
  if(p->value<value) { func_00235798((SkatQueueOwner *)((char*)obj+4),smallest); }
  else goto end;
 }
 p->value=value;
end:
 return result;
}
