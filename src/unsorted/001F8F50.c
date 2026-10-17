/* UNIT 0x001F8F50 0x001F9118 .sbss=0x29E128:0x29E138 */
#include "ui_style.h"
int D_0029E128 __attribute__((section(".sbss"))) = 0;
int D_0029E12C __attribute__((section(".sbss"))) = 0;
int D_0029E130 __attribute__((section(".sbss"))) = 0;
int D_0029E134 __attribute__((section(".sbss"))) = 0;
extern int D_003F52F0[];
extern int *D_0029E04C;
extern char D_004C6F10[],D_004C6F30[],D_004C6F50[],D_004C6F70[],D_004B25A0[];
extern void func_0020F9A8(int,int,int,int,int),func_001BAF10(void *,int),func_00218498(int,int);
extern int func_0018D860(void *),func_0013AF60(int);
void func_001F8F50(int *obj) {
 int id=D_003F52F0[0];
 if(!D_0029E128) { func_0018D7D0((UiStyle *)(void *)(D_004C6F10),414,198,-143,-26,80,80,80,255); D_0029E128=1; }
 if(!D_0029E12C) { func_0018D7D0((UiStyle *)(void *)(D_004C6F30),414,198,-143,-26,153,119,46,255); D_0029E12C=1; }
 if(!D_0029E130) { func_0018D7D0((UiStyle *)(void *)(D_004C6F50),304,144,-113,-27,66,66,66,255); D_0029E130=1; }
 if(!D_0029E134) { func_0018D7D0((UiStyle *)(void *)(D_004C6F70),366,160,-63,27,24,57,89,255); D_0029E134=1; }
 func_0018D800((const UiStyle *)(void *)(D_004C6F10));
 func_0020F9A8(0,id,30,15,D_0029E04C[1]);
 func_001BAF10(D_004B25A0,2);
 func_0018D800((const UiStyle *)(void *)(D_004C6F30));
 func_0020F9A8(0,id,30,32,func_0018D860((char *)obj+(*obj*16+24)));
 func_00218498(func_0013AF60(0x25b0),*obj+1);
}
