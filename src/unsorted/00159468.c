/* UNIT 0x00159468 0x00159518 */
#include "animation_track_search.h"
#if defined(__mips__)
#define CallShared00163C58_0 func_00163C58
#else
#define CallShared00163C58_0 Forward00163C58_0
#endif
struct Obj {char pad[0x28CC];float x28CC;char pad28D0[0x60];int x2930,x2934;};
struct Global {char pad[0x50198];float x50198;};
extern struct Global D_002AF2C0;
extern char D_003041B0[];
extern int func_00163C58(void *,float,int,int);
extern int func_00163D40(void *,int);
int func_00159468(struct Obj *p,int a) {
 float value=p->x28CC;float limit=D_002AF2C0.x50198-200.f;float minimum;int result=0,index;void *table;
 __asm__ volatile("min.s %1,%0,%2":"+f"(value),"=&f"(minimum):"f"(limit):"memory");
 p->x28CC=minimum;
 if(p->x2934!=a){p->x28CC=limit;p->x2930=-1;p->x2934=a;}
 table=D_003041B0;
 value=p->x28CC;index=p->x2930;
 index=CallShared00163C58_0(table,value,index,a);p->x2930=index;
 if(index>=0)result=func_00163D40(table,index);
 return result;
}
