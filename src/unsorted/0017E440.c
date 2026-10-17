/* UNIT 0x0017E440 0x0017E670 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0017ED50_16_func_0017ED50 func_0017ED50
#else
#define CallScalarBridge0017ED50_16_func_0017ED50 ScalarBridge0017ED50_16
#endif
struct Flags { char pad0[0x104]; unsigned char x104,x105; };
struct Rider { char pad0[0x78D0]; struct Flags *x78D0; };
struct Obj { struct Rider *x0; char pad4[0x1BC]; int x1C0,x1C4; char pad1C8[0x5C]; int x224,x228; char pad22C[0x1C0]; int x3EC; char pad3F0[0x4D]; unsigned char x43D; };

#if defined(__mips__)
extern int func_0017ED50(struct Obj *,int,int);
#else

#endif

extern void func_0017EB00(struct Obj *,int,int);
extern int func_002204A8(int);
extern int func_0017E670(struct Obj *,float *);
extern int abs(int);
extern int func_00222BD8(int,int);
void func_0017E440(struct Obj *obj,int input) {
 float value; struct Flags *flags;
 if(CallScalarBridge0017ED50_16_func_0017ED50(obj,11,12)) {
  if(obj->x224) {
   func_0017EB00(obj,11,input); func_0017EB00(obj,12,input); obj->x1C4=0;
  } else if(obj->x228) {
   func_0017EB00(obj,12,input); func_0017EB00(obj,11,input); obj->x1C0=0;
  }
  if(!obj->x1C0 && !obj->x1C4) {
   func_0017EB00(obj,11,input);
   if(!obj->x1C0) {
    int axis; axis=(signed char)(func_002204A8(input)>>16);
    axis=abs(axis);
    if(axis>=121) obj->x1C0=1;
   }
   if(obj->x1C0) {
    obj->x1C0=0;
    flags=obj->x0->x78D0;
    if(flags) {
     if(flags->x104) goto pick_left;
     if(flags->x105) goto pick_right;
    }
    if(func_0017E670(obj,&value)) {
     if(value>0.0f) obj->x1C4=1; else obj->x1C0=1;
    } else if(obj->x3EC<30) {
     if(obj->x43D) obj->x1C0=1; else obj->x1C4=1;
    } else if(func_00222BD8(0,1)) obj->x1C0=1;
    else obj->x1C4=1;
   }
  }
 } else {
  int axis;
  func_0017EB00(obj,11,input); func_0017EB00(obj,12,input);
  axis=(signed char)(func_002204A8(input)>>16);
  if(axis<-120) { pick_left: obj->x1C0=1; } else if(axis>120) { pick_right: obj->x1C4=1; }
 }
 if(obj->x1C0) { obj->x3EC=0; obj->x43D=1; }
 else if(obj->x1C4) { obj->x3EC=0; obj->x43D=0; }
 else obj->x3EC++;
}
