/* UNIT 0x001845A8 0x00184630 */
#include "tween.h"
typedef struct {char *x0; int x4,x8,xc,x10,x14,x18,x1c;} Obj;
extern void func_00202E38(void *,int,int,float);


extern int func_00238F48(void);
void func_001845A8(Obj *o){char *p=o->x0;
func_00202E38(p+0x2144,1,1,(float)o->x1c);
func_001B3170((RiderSecondaryBlend *)(p+0x3530), o->x1c, 1.0f);
func_001AE630((RiderBodyBlend *)(p+0x47f0), o->x1c, 1.0f);
o->x8=0; o->x10=func_00238F48(); *(int *)(p+0x6300)=60;
}
