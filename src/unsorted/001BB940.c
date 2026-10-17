/* UNIT 0x001BB940 0x001BB9E0 */
#include "tween.h"
typedef struct Obj { char *x0; char pad4[12]; float x10; } Obj;
extern void func_00202E38(void *,int,int,float);


void func_001BB940(Obj *obj) {
    char *p=obj->x0;
    func_00202E38(p+0x2144,1,1,obj->x10);
    func_00202E38(p+0x2F0,1,1,obj->x10);
    func_001B3170((RiderSecondaryBlend *)(p+0x3530), (int)(obj->x10+obj->x10), 1.0f);
    func_001AE630((RiderBodyBlend *)(p+0x47F0), (int)(obj->x10+obj->x10), 1.0f);
}
