/* UNIT 0x00191A28 0x00191BC0 */
#include "object_registry.h"
#include "scene.h"
typedef struct Value { int x[4]; } Value;
typedef struct Bits { unsigned int low:7; unsigned int id:11; unsigned int high:14; } Bits;
typedef SceneNode Other;
typedef struct Obj { char pad[8]; Other *x8; int xC, x10, x14; Value x18,x28,x38; char pad2[0x38]; int x80,x84,x88; } Obj;
extern void func_00191BC0(Obj *,int);
extern void func_00203590(Value *,int,int,int);
extern void func_00203CB8(Value *);

extern void func_00192118(Obj *,int);
void func_00191A28(Obj *obj,int a,Other *other,int b,int c) {
 Value v;
 obj->x10=a; obj->x8=other;
 func_00191BC0(obj,0);
 func_00203590(&obj->x18,0x4D0,b,1);
 func_00203590(&obj->x28,0x4D0,b,2);
 func_00203590(&obj->x38,0x4D0,b,3);
 if(obj->x18.x[0]) {v=obj->x18;func_00203CB8(&v);}
 if(obj->x28.x[0]) {v=obj->x28;func_00203CB8(&v);}
 if(obj->x38.x[0]) {v=obj->x38;func_00203CB8(&v);}
 if(obj->x8 && !obj->x8->xB4) {
  obj->x80=b;
  obj->x8->xB4=RegisterObjectRetail((void *)(obj), (unsigned int)(((Bits *)obj->x8->resource)));
  ((Bits *)obj->x8->resource)->id=obj->x8->xB4;
  func_00192118(obj,c);
 }
 obj->x88=3; obj->x84=-1;
}
