/* UNIT 0x0017A1B0 0x0017A250 */
struct Obj { char pad[0x10]; float x10; char pad2[0xC]; int x20,x24; };
extern void func_0017A250(void *,int,int,int);
void func_0017A1B0(void *self,struct Obj *obj,unsigned char *count,int direction,int update,float delta) {
 if(obj) {
  if(direction > 0) {
   if(*count < 20) { obj->x10 -= delta; ++*count; }
   if(update) func_0017A250(self,direction,obj->x20,obj->x24);
  } else if(*count) {
   obj->x10 += delta; --*count;
   if(update) func_0017A250(self,direction,obj->x20,obj->x24);
  }
 }
}
