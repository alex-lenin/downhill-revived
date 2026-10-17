/* UNIT 0x00147BD8 0x00147D90 */
struct Node { char pad[12]; struct Node *next; };
struct Obj { int count; struct Node *head; float phase,amount,step,x14,x18,x1C; int timer; };
extern void func_00147F40(struct Node *,int,float,float);
int func_00147BD8(struct Obj *obj) {
 int i=0; struct Node *node; float t;
 obj->phase += obj->step;
 if(obj->phase>1.0f) obj->phase-=2.0f;
 node=obj->head;
 while(node) {
  t=(float)i/((float)obj->count-1.0f);
  func_00147F40(node,i,obj->phase,(t-t*t)*4.0f);
  i++; node=node->next;
 }
 if(obj->timer) {
  float value;
  obj->timer--;
  value=obj->amount+0.01f;
  if(value<=1.0f) obj->amount=value;
 } else {
  float value;
  value=obj->amount-0.01f;
  if(value>=0.0f) obj->amount=value;
 }
 obj->x14=obj->amount*-30.0f+60.0f;
 obj->x18=obj->amount+obj->amount+1.0f;
 obj->x1C=obj->amount*10.0f+5.0f;
 obj->step=0.01f;
 if(obj->timer<0) obj->timer=0;
 return 1;
}
