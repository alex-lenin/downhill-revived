/* UNIT 0x00152C58 0x00152D38 */
#include "tween.h"
struct Obj { char *x0; char pad4[8]; float xC; int x10,x14; };
extern void func_001525E8(struct Obj *obj);



extern void func_001D06C0(void *,int);
void func_00152C58(struct Obj *obj) {
 char *base,*part,*target; int mode; float value;
 func_001525E8(obj);
 base=obj->x0; mode=obj->x14; part=base+0x1E40;
 if(mode) {
 if(*(unsigned char *)(part+0x2160)) {
 value=obj->xC; target=base+0x3AF0;
 if(mode==0x15 || mode==0x86) func_001B1698((RiderBlendController *)(base+0x3C00), (int)value, 0.0f);
 else func_001B1698((RiderBlendController *)(target), (int)value, 0.0f);
 func_001B3170((RiderSecondaryBlend *)(part+0x16F0), (int)value, 0.25f);
 SetBodyBlendWeightFirst((RiderBodyBlend *)(base+0x47F0), 0.25f, (int)value);
 }
 func_001D06C0(part+0x2168,0);
 }
}
