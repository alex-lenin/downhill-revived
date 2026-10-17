/* UNIT 0x001E9CC0 0x001E9F10 */
#include "tween.h"
typedef struct { int x0, x4, x8, xC, x10; float x14,x18; char pad[12]; } Entry;
typedef struct { char pad[0x3FA0]; unsigned char x3FA0; } Owner;
typedef struct { Owner *x0; int x4; unsigned char x8; char pad[0x23]; int x2C,x30; } Object;
extern Entry D_002736A8[], D_00274BD0[];



void func_001E9CC0(Object *obj,int mode,float value) {
 Entry *a = &D_002736A8[obj->x2C];
 Entry *b = &D_00274BD0[obj->x30];
 Owner *owner = obj->x0;
 if(owner->x3FA0) {
 char *part = (char *)owner+0x3530;
 char *other = (char *)owner+0x3AF0;
 if(mode) {
 if(b->x4 && a->x4 != b->x4) func_001B1698((RiderBlendController *)((char *)owner+0x3E20), (int)value, 1.0f);
 if(b->x8 && a->x8 != b->x8) func_001B1698((RiderBlendController *)(other+0x220), (int)value, 1.0f);
 if(b->xC && a->xC != b->xC) func_001B1698((RiderBlendController *)(other+0x110), (int)value, 1.0f);
 if(b->x10 && a->x10 != b->x10) func_001B1698((RiderBlendController *)(other), (int)value, 1.0f);
 } else {
 if(a->x4) func_001B1698((RiderBlendController *)((char *)owner+0x3E20), (int)value, 1.0f);
 if(a->x8) func_001B1698((RiderBlendController *)((char *)owner+0x3D10), (int)value, 1.0f);
 if(a->xC) func_001B1698((RiderBlendController *)((char *)owner+0x3C00), (int)value, 1.0f);
 if(a->x10) func_001B1698((RiderBlendController *)(other), (int)value, 1.0f);
 if(a->x14 < 1.0f) func_001B3170((RiderSecondaryBlend *)(part), (int)value, 1.0f);
 if(a->x18 < 1.0f) func_001AE630((RiderBodyBlend *)((char *)owner+0x47F0), (int)value, 1.0f);
 obj->x8=0;
 }
 }
}
