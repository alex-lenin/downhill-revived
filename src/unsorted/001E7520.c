/* UNIT 0x001E7520 0x001E75D8 */
#include "animation.h"
typedef struct { char p[0x16D4]; int **x16D4; char q[0xA8C]; int x2164; } Sub; typedef struct { char p[0x1E40]; Sub sub; } Parent;
typedef struct { Parent *x0; char p4[4]; char x8; char p9[3]; int xC; char x10; char p11[0x13]; float x24; char p28[12]; int x34,x38; char p3C[0x190]; int x1CC; char p1D0[0x20]; char x1F0; char p1F1[4]; char x1F5,x1F6; char pad1F7; AnimationHandle x1F8,x208; } Obj;
 extern void func_001E9998(Obj*);
void func_001E7520(Obj *o) { Sub *sub; o->x24=1.f; o->x34=0; o->x38=0; o->x8=0; o->xC=0; o->x10=0; o->x1F0=0; o->x1F5=0; o->x1F6=0; o->x1CC=0; sub=&o->x0->sub; if((**sub->x16D4&0xFFFC0000)==0x08400000) { func_002036C8(&o->x1F8,0x3ED,0,0,sub->x2164); func_001E9998(o); func_002036C8(&o->x208,0x3ED,0,0xFD,o->x0->sub.x2164); } }
