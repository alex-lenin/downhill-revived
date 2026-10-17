/* UNIT 0x00171050 0x00171128 */
#include "tween.h"
extern void func_001ACFA0(void *, int);
extern void func_00136030(void *, int);


extern void func_001AE210(void *, float);

void func_00171050(char **obj) {
 char *p = *obj;
 int *x48b0 = (int *)(p + 0x48b0);
 char *x3af0;
 func_001ACFA0(p + 0x4580, 0);
 x3af0 = p + 0x3af0;
 *x48b0 = 0;
 func_00136030(p + 0x40c0, 1);
 func_001B3170((RiderSecondaryBlend *)(p + 0x3530), 240, 1.0f);
 func_001AE630((RiderBodyBlend *)(p + 0x47f0), 240, 1.0f);
 func_001AE210(x48b0, 240.0f);
 func_001B1698((RiderBlendController *)(p + 0x3c00), 240, 1.0f);
 func_001B1698((RiderBlendController *)(x3af0), 240, 1.0f);
 func_001B1698((RiderBlendController *)(p + 0x3d10), 240, 1.0f);
 func_001B1698((RiderBlendController *)(p + 0x3e20), 240, 1.0f);
}
