/* UNIT 0x0012A6D8 0x0012A790 */
#include "rider_geometry.h"
typedef struct Obj { int x0,x4; char pad8[0x178-8]; int x178,x17C,x180; } Obj;
extern void func_0012AC28(Obj *);
extern void func_0012ACC8(Obj *);
extern void func_0012AD60(Obj *);
extern void func_0012B188(Obj *);
extern void func_00129FC8(Obj *);

extern float func_00238F40(void);
extern void func_00236558(void *,void *,float);
extern void func_00162690(void *,int,int,float);
void func_0012A6D8(Obj *obj) {
 float vector[4] __attribute__((aligned(16)));
 void *position;
 int active;
 func_0012AC28(obj);
 if(obj->x178) func_0012ACC8(obj);
 if(obj->x180) func_0012AD60(obj);
 position=(char *)obj+0xD0;
 func_00260E08((char *)obj+0x80, position, (char *)obj+0xE0, vector);
 func_00236558(position,vector,func_00238F40());
 func_0012B188(obj); func_00129FC8(obj);
 
 func_00162690((char *)obj+0x110,obj->x180!=0 && obj->x180<5,obj->x4,1.0f);
}
