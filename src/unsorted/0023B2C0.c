/* UNIT 0x0023B2C0 0x0023B420 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0023B220_32_func_0023B220 func_0023B220
#else
#define CallScalarBridge0023B220_32_func_0023B220 ScalarBridge0023B220_32
#endif
typedef struct Entry { long x0,x8; int x10,x14; } Entry;
typedef struct Obj { unsigned int x0,x4; int x8,xC,x10,x14; char pad18[0x2C]; int x44; char pad48[8]; Entry *x50; int x54,x58,x5C; } Obj;

#if defined(__mips__)
extern void func_0023B220(unsigned int,unsigned int,int,int);
#else

#endif

extern void func_0023B1B0(int);
int func_0023B2C0(Obj *o) {
 int i=0;
 o->x44=1; o->xC=0; o->x10=0; o->x14=0; o->x58=0; o->x5C=0;
 for(;i<o->x54;i++) { o->x50[i].x0=-1; o->x50[i].x8=-1; o->x50[i].x10=0; o->x50[i].x14=0; }
 for(i=0;i<o->x8;i++) CallScalarBridge0023B220_32_func_0023B220(o->x4+i*16,((i<<11)+o->x0)&0xFFFFFFF,3,128);
 CallScalarBridge0023B220_32_func_0023B220(o->x4+i*16,o->x4&0xFFFFFFF,2,0);
 *(volatile unsigned int *)0x1000B420=0;
 *(volatile unsigned int *)0x1000B410=o->x0&0xFFFFFFF;
 *(volatile unsigned int *)0x1000B430=o->x4&0xFFFFFFF;
 func_0023B1B0(5); return 1;
}
