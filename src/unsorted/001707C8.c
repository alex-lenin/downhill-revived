/* UNIT 0x001707C8 0x00170850 */
#include "tween.h"
extern void func_00202E38(void *,int,int,float);


void func_001707C8(char **obj) {
 char *inner=*obj;
 func_00202E38(inner+0x2144,1,1,100.0f);
 func_00202E38(inner+0x2F0,1,1,100.0f);
 func_001B3170((RiderSecondaryBlend *)(inner+0x3530), 200, 1.0f);
 func_001AE630((RiderBodyBlend *)(inner+0x47F0), 200, 1.0f);
}
