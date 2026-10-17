/* UNIT 0x0018E518 0x0018E5A8 */
typedef struct Item Item;
typedef struct VTable { char pad[0x10]; short delta; short pad12; int (*call)(void *); } VTable;
struct Item { char pad[0x10]; VTable *vtable; };
typedef struct Obj { char pad[0x424]; Item *items[4]; int count; } Obj;
extern char D_003F52D0[];
extern void func_001904B8(void *, Item *);
void func_0018E518(Obj *obj) {
 int i;
 Item **item;
 for(i=0; i<obj->count; i++) {
 Item *p;
 VTable *v;
 item=&obj->items[i];
 p=*item;
 v=p->vtable;
 if(v->call((char *)p+v->delta)) func_001904B8(D_003F52D0,*item);
 }
}
