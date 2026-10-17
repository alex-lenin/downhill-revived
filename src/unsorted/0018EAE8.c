/* UNIT 0x0018EAE8 0x0018EB50 */
typedef struct Obj Obj;
typedef struct { short delta; short pad; void (*call)(void *,int,int,int); } VEntry;
struct Obj { char pad[0x4F8]; VEntry *vtable; };
extern Obj *func_0018EAC8(void *,int);
void func_0018EAE8(int *obj,int a,int b,int c) {
 Obj *target=func_0018EAC8(obj,*obj);
 if(target) {
  VEntry *table=target->vtable;
  table[4].call((char *)target+table[4].delta,a,b,c);
 }
}
