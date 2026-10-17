/* UNIT 0x001BE638 0x001BE778 */
#include "skat_queue.h"
typedef struct Obj { int x0,x4,x8; } Obj;
typedef struct Entry { int x0; char pad[0x80]; } Entry;
typedef struct Global { char pad[0x5350]; int x5350[1]; int x5354[1]; char pad5358[0x62c]; Entry entries[5]; } Global;
extern Global D_00398C40[];
extern int func_001BC618(void *, int);

extern void func_001BE778(Obj *,int,int);
extern void func_001BECD0(Obj *,int);
static __inline__ void *member(Global *g, int offset) { return (char *)g + offset; }
static __inline__ void remove_entry(Global *g, int key) { func_00235798((SkatQueueOwner *)((char *)g + 0x5354),key); }
static __inline__ void mark(Global *g, int index) { g->entries[index].x0 = 1; }
static __inline__ int lookup(Global *g, int key) { return func_001BC618((char *)g + 0x5350,key); }
void func_001BE638(Obj *obj,int a,int b,int mode) {
 int first, second;
 first=lookup(D_00398C40,obj->x0);
 second=lookup(D_00398C40,obj->x8);
 if(mode==6) {
  if(first!=5) remove_entry(D_00398C40,first);
  if(second==5) func_001BE778(obj,a,b);
 } else if(mode==3 || mode==5 || mode==2 || mode==4) {
  if(second!=5) remove_entry(D_00398C40,second);
  if(first==5) func_001BECD0(obj,a);
  else mark(D_00398C40,first);
 }
}
