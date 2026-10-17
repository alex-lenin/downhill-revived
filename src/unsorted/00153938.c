/* UNIT 0x00153938 0x001539F8 */
#include "tween.h"
struct Obj { char *x0; char pad4[8]; float xC; int x10,x14; char pad18[16]; int x28; };
extern void func_001525E8(struct Obj *);
extern void func_001AC2E8(void *,int);



extern void func_001D06C0(void *,int);
void func_00153938(struct Obj *obj) {
 char *base,*sub,*part; int index,flag;
 func_001525E8(obj);
 base=obj->x0; sub=base+0x1e40;
 func_001AC2E8(sub,1);
 flag=obj->x14;obj->x28=flag;
 if(flag && *(unsigned char *)(sub+0x2160)) {
  index=(int)obj->xC; part=base+0x3530;
  func_001B1698((RiderBlendController *)(base+0x3af0), index, 0.0f);
  SetSecondaryBlendWeightFirst((RiderSecondaryBlend *)(part), 0.25f, index);
  SetBodyBlendWeightFirst((RiderBodyBlend *)(base+0x47f0), 0.25f, index);
 }
 func_001D06C0(sub+0x2168,3);
}
