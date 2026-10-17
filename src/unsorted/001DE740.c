/* UNIT 0x001DE740 0x001DE8D0 */
#include "scalar_api.h"
#include "skat_queue.h"
#include "sound.h"
typedef struct Obj {char pad0[0x70]; int x70,x74; char pad78[0x54]; int xcc[43]; unsigned int x178;} Obj;
extern struct Global {char pad[0x5354]; char x5354[4];} D_00398C40;
extern void func_00235878(void*,int);
extern int func_002013F8(void);

#if defined(__mips__)
extern void func_00202688(int,int,int,int,int,int,int,int,int,int,void(*)(void),unsigned long);
#else

#endif

extern void func_001DF600(void*);
static inline void *get_global(struct Global *p) {return p->x5354;}
void func_001DE740(Obj* p) {
 unsigned int h; int i;
 if(p->x70) {
  h=p->x178;
  if(h==0) {
   {void *g=get_global(&D_00398C40); for(i=0;i<4;i++){func_00235798((SkatQueueOwner *)(g),i);func_00235878(g,i);}}
   while(func_002013F8()) {}
   p->x178=0xFFFFFFFF;
   func_00202688(p->xcc[p->x74],0,0,0,0x400,0,1,0,0,4,func_00208C50,(unsigned long)(unsigned int)&p->x178);
  }else if(h!=0xFFFFFFFF && h!=0){p->x178=0xFFFFFFFF;func_00201D80(h, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&p->x178));while(func_002013F8()) {}}
  func_002013F8();
 }else {unsigned int other=p->x178;if(other!=0xFFFFFFFF && other!=0)func_001DF600(&p->x178);}
}
