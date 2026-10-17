/* UNIT 0x00181D60 0x00181DE0 */
#include "tween.h"
typedef struct Obj { char *x0; int x4,x8,xC,x10,x14; } Obj;
extern void func_00202E38(char *, int, int, float);


void func_00181D60(Obj *obj) {
 char *base=obj->x0;
 func_00202E38(base+0x2144,1,1,(float)obj->x14);
 func_001B3170((RiderSecondaryBlend *)(base+0x3530), obj->x14, 1.0f);
 func_001AE630((RiderBodyBlend *)(base+0x47F0), obj->x14, 1.0f);
 obj->x8=0; obj->xC=0;
}
