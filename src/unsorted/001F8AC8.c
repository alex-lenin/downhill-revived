/* UNIT 0x001F8AC8 0x001F8CA8 .sbss=0x29E118:0x29E128 */
#include "ui_style.h"
struct Obj { int x0; char pad4[0x14]; char x18[1]; };
struct State {char pad[0x290]; int x290;};
struct Config {int x0; int x4;};
int D_0029E118 __attribute__((section(".sbss"))) = 0;
int D_0029E11C __attribute__((section(".sbss"))) = 0;
int D_0029E120 __attribute__((section(".sbss"))) = 0;
int D_0029E124 __attribute__((section(".sbss"))) = 0;
extern int D_003F52F0[];
extern struct Config *D_0029E04C;
extern char D_004C6E90[], D_004C6EB0[], D_004C6ED0[], D_004C6EF0[];
extern struct State D_004B25A0;
extern int func_0018D860(void *);
extern void func_001BAF10(void *,int);
extern void func_0020F9A8(int,int,int,int,int);
extern int func_001F7AC8(int);
extern void func_001F57D0(int,int);
static inline void Mark(int *flag, int value) {*flag=value;}
static inline int ReadConfig(struct Config **p) {return (*p)->x4;}
static inline void InitThird(void) {if (!D_0029E120) {func_0018D7D0((UiStyle *)(void *)(D_004C6ED0),304,144,-113,-27,66,66,66,255); Mark(&D_0029E120,1);}}
static inline void InitFourth(void) {if (!D_0029E124) {func_0018D7D0((UiStyle *)(void *)(D_004C6EF0),366,160,-63,27,24,57,89,255); Mark(&D_0029E124,1);}}
void func_001F8AC8(struct Obj *obj) {
 int value = D_003F52F0[0];
 if (!D_0029E118) {func_0018D7D0((UiStyle *)(void *)(D_004C6E90),414,198,-143,-26,80,80,80,255); D_0029E118=1;}
 if (!D_0029E11C) {func_0018D7D0((UiStyle *)(void *)(D_004C6EB0),414,198,-143,-26,153,119,46,255); D_0029E11C=1;}
 InitThird();
 InitFourth();
 func_0018D800((const UiStyle *)(void *)(D_004C6E90));
 func_0020F9A8(0,value,30,10,ReadConfig(&D_0029E04C));
 func_001BAF10(&D_004B25A0,2);
 func_0018D800((const UiStyle *)(void *)(D_004C6EB0));
 func_0020F9A8(0,value,30,25,func_0018D860((obj->x0*16+24)+(char *)obj));
 func_001F57D0(func_001F7AC8(D_004B25A0.x290)*10+0x2354,obj->x0);
 func_001F57D0(0x23d6,obj->x0);
}
