/* UNIT 0x001B11C8 0x001B12A0 */
#include "animation.h"
typedef AnimationHandle Desc;
struct Parent { char pad[0x2164]; int x2164; };
struct Obj { char pad[0x58]; union { unsigned long flags; struct {int x58; void (*x5C)(void);}; } state; struct Obj *x60; char pad64[0xC]; Desc x70; char pad80[0x8C]; struct Parent *x10C; };
extern void func_001B1748(void);


extern void func_001B12B0(void *,int,int,int);
void func_001B11C8(struct Obj *obj,int arg1,int arg2,int arg3,int arg4) {
 Desc desc;
 func_002037D0(&obj->x70,arg1,0,obj->x10C->x2164);
 obj->state.flags=(obj->state.flags & ~0x3FL)|0x2B;
 obj->x70.x8=(int)&obj->state;
 obj->state.x5C=func_001B1748;
 obj->x60=obj;
 desc=obj->x70;
 func_002033E0(&desc,obj,2,10,1.0f);
 func_001B12B0(obj,arg2,arg3,arg4);
}
