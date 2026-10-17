/* UNIT 0x00235BC8 0x00235DE8 */
#include "scalar_api.h"
struct Entry { int x0,x4; unsigned int x8; int xC,x10,x14; unsigned int x18; char pad[0x108]; };
struct List { int x0; struct Entry entries[50]; char pad[0x4]; int x3910,x3914; unsigned int x3918; int x391C,x3920; unsigned char x3924; };
struct Vtable { char pad[0x48]; short delta; short pad2; void (*call)(void *,struct Entry *,int *,int *,unsigned char *,int *); };
struct Obj { char pad[0x5FC]; struct Vtable *vt; int x600; };
extern char D_00299330[];
extern void func_00254D08(char *);

#if defined(__mips__)
extern void func_00235E18(void);
#else

#endif


#if defined(__mips__)
extern void func_00202688(int,int,int,int,int,int,int,int,int,int,void (*)(void),long);
#else

#endif

extern void func_00202658(int,int,int,int,int,int,int,int,int,int);
void func_00235BC8(struct Obj *obj, struct List *list) {
 unsigned char b; int x,y,flags=0; int rem2=0,bits=0,sector2=0; struct Entry *entry=&list->entries[list->x3910]; unsigned int size; int base,kind,start,old,sector,rem;
 if ((unsigned int)(entry->x10-11)>=2 && entry->x10!=5) { func_00254D08(D_00299330); return; }
 if(list->x3920==-1) list->x3920=entry->x4; else if(list->x3920!=entry->x4) list->x3920=-1;
 obj->vt->call((char *)obj+obj->vt->delta,entry,&x,&y,&b,&flags);
 if(b) flags|=2;
 if(flags&0x40) list->x3924=1; else list->x3924=1;
 size=entry->x8; kind=entry->x10; start=entry->x14; old=list->x3918;
 while(size) {size>>=1; bits++;}
 rem=start; sector=rem/2048; base=obj->x600; sector+=base; rem%=2048;
 if(kind==12) {rem2=start+(entry->x18>>1); sector2=rem2/2048; sector2+=base; rem2%=2048;}
 if(!old) { list->x3918=0xFFFFFFFFU; flags|=1; func_00202688(sector,sector2,rem,rem2,x,y,list->x391C,0,bits,flags,func_00235E18,(long)list); }
 else func_00202658(sector,sector2,rem,rem2,x,y,list->x391C,old,bits,flags);
}
