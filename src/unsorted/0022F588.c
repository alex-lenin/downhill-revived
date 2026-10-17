/* UNIT 0x0022F588 0x0022F5E0 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct { int x0,x4,x8,xC; u128 *x10; u128 **x14; } Obj;
void func_0022F588(volatile Obj *obj,u128 *buffer,u128 **table) {
 int count; u128 *p,*prev; u128 **slot;
 obj->x10=buffer;obj->x14=table;
 p=buffer+128;slot=table+128;count=128;
 do {
  prev=p-1;
  *(int *)p=0x20000000;
  *(unsigned int *)((char *)p+4)=(int)prev&0xfffffff;
  count--;
  *slot=p;p=prev;slot--;
 }while(count>=0);
 *obj->x10=(u128)0x70000000;
}
