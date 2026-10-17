/* UNIT 0x001DDDB0 0x001DDE70 */
#include "scalar_api.h"
#include "sound.h"
struct Obj { char pad[0x16c]; int x16c,x170; unsigned int x174; };

#if defined(__mips__)
extern void func_00202688(int,int,int,int,int,int,int,int,int,int,void (*)(void),unsigned long);
#else

#endif

extern void func_002013F8(void);
extern void func_001BC4F0(void*);
extern struct { char pad[0x5350]; int x5350; } D_00398C40;
static inline void finish(void *base) { func_001BC4F0((char*)base+0x5350); }
void func_001DDDB0(struct Obj *obj) {
 unsigned int value=obj->x174;
 if(value==0) {
  obj->x174=0xffffffffU;
  func_00202688(obj->x16c,obj->x170,0,0,0x266,0,1,0,0,4,func_00208C50,(unsigned long)(unsigned int)&obj->x174);
 } else if(value!=0xffffffffU && value!=0) {
  obj->x174=0xffffffffU;
  func_00201D80(value, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->x174));
 }
 func_002013F8();
 finish(&D_00398C40);
}
