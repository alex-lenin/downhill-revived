/* UNIT 0x0020ECD0 0x0020ED30 */
#include "scene_effects.h"
struct State { float x0,x4,x8,xC,x10,x14,x18; int x1C; };
extern struct State D_004F3200;
void func_0020ECD0(int a,int b,int c,float d,float e,float f,float g) {
    D_004F3200.x0=d;
    D_004F3200.x4=e;
    D_004F3200.x8=g;
    D_004F3200.xC=f;
    D_004F3200.x10=(float)a;
    D_004F3200.x14=(float)b;
    D_004F3200.x18=(float)c;
    D_004F3200.x1C=1;
}
