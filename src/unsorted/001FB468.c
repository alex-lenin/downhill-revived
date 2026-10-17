/* UNIT 0x001FB468 0x001FB508 */
#include "camera.h"
typedef struct { char pad[0x38]; int x38; } Sub;
typedef struct { char pad[0x3530]; char x3530[0xB90]; Sub x40C0; char pad40FC[0x2A4]; char x43A0[0x80]; char x4420[1]; } Owner;
typedef struct { Owner *x0; int x4; float x8; char padC[0x10]; float x1C; char pad20[8]; int x28; } Obj;
extern void func_00134D70(void *, void *, Sub *);
extern void func_00232C98(Sub *, void *, int, float);

void func_001FB468(Obj *obj) {
 float vec[4];
 float value;
 Owner *owner = obj->x0;
 Sub *sub = &owner->x40C0;
 if (obj->x28 == 0) {
  value = obj->x1C;
  func_00134D70(owner->x4420, vec, sub);
 } else {
  value = -obj->x1C;
  func_00134D70(owner->x43A0, vec, sub);
 }
 func_00232C98(sub, vec, sub->x38, value);
 func_001B3028((RiderCamera *)(owner->x3530), 12, obj->x8);
}
