/* UNIT 0x00163148 0x00163218 */
#include "tween.h"
typedef struct { char *x0; char pad4[12]; float x10; } Obj;
extern void func_00202E38(void *, int, int, float);



void func_00163148(Obj *obj) {
 char *base = obj->x0;
 void *part;
 func_00202E38(base + 0x2144, 1, 1, obj->x10);
 part = base + 0x3530;
 func_00202E38(base + 0x2F0, 1, 1, obj->x10);
 func_001B1698((RiderBlendController *)(base + 0x3AF0), (int)obj->x10, 1.0f);
 func_001B1698((RiderBlendController *)(base + 0x3C00), (int)obj->x10, 1.0f);
 func_001B3170((RiderSecondaryBlend *)(part), (int)obj->x10, 1.0f);
 func_001AE630((RiderBodyBlend *)(base + 0x47F0), (int)obj->x10, 1.0f);
}
