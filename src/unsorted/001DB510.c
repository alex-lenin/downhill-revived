/* UNIT 0x001DB510 0x001DB6A0 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));
extern void func_001DB6B0(void*,int,int,int*,int*,int);
extern void func_00208BF8(void*,void*);
extern char D_004BD840[];
extern int D_004BD8A0[];
void func_001DB510(SoundEmitter* p) {
 int count=0, value=0;
 func_001DB6B0(&p->position,p->nearDistance,p->farDistance,&count,&value,0);
 if(count>0) {
  unsigned int handle=p->handle;
  if(handle==0) {p->handle=0xFFFFFFFF; func_00201BD0(D_004BD8A0[0], 0, p->name, count, value, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)p));}
  else if(handle!=0xFFFFFFFF && handle!=0) {p->handle=0xFFFFFFFF;func_00201E08(handle, 5, count, value, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(p));}
  if(p->x38) {
   if(count==0x400 && p->secondaryHandle==0) {
    p->secondaryHandle=0xFFFFFFFF;
    func_00201BD0(D_004BD8A0[0], 0, ((u128)0x38305245<<64) | (u128)0x53555F4D55445F50ULL, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&p->secondaryHandle));
   } else if(count<0x400) func_00208BF8(D_004BD840,&p->secondaryHandle);
  }
 }else func_00208BF8(D_004BD840,p);
}
