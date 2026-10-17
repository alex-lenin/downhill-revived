/* UNIT 0x00152768 0x001528C0 */
#include "tween.h"
struct Flags { char p[0x440]; unsigned long flags; };
struct Base { char p[0x3fa0]; unsigned char x3FA0; char q[0x3933]; struct Flags *x78D4; };
struct Obj { struct Base *base; char p4[12]; float x10; int x14; unsigned char x18; char p19; unsigned char x1A; char p1b[5]; unsigned char x20,x21; };



extern void func_00202E38(void *,int,int,float);
void func_00152768(struct Obj *obj) {
 struct Base *base; struct Flags *flags; char *sub,*part;
 base=obj->base; flags=base->x78D4; sub=(char *)base+0x1e40;
 if(flags) flags->flags=(flags->flags & 0xfffffffdffffffffUL) | 0x200000000UL;
 obj->x18=0; obj->x1A=0; obj->x14=0;
 if(*(unsigned char *)(sub+0x2160)) {
 part=(char *)base+0x3af0;
 func_001B3170((RiderSecondaryBlend *)((char *)base+0x3530), (int)obj->x10, 1.0f);
 func_001AE630((RiderBodyBlend *)((char *)base+0x47f0), (int)obj->x10, 1.0f);
 if(obj->x20) {
 func_001B1698((RiderBlendController *)(part), (int)obj->x10, 1.0f);
 func_001B1698((RiderBlendController *)((char *)base+0x3c00), (int)obj->x10, 1.0f);
 }
 if(obj->x21) {
 func_001B1698((RiderBlendController *)((char *)base+0x3d10), (int)obj->x10, 1.0f);
 func_001B1698((RiderBlendController *)((char *)base+0x3e20), (int)obj->x10, 1.0f);
 }
 func_00202E38((char *)base+0x2144,1,1,obj->x10);
 func_00202E38((char *)base+0x2f0,1,1,obj->x10);
 }
}
