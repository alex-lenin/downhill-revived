/* UNIT 0x0020EE30 0x0020EFE8 */
typedef struct { float x0,x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28; } Obj;
extern float func_00239030(void);
extern void func_0020EC78(float *,float *,float *,float *,int *,int *,int *);
#include "scene_effects.h"
extern void func_00216F48(float);
void func_0020EE30(Obj *obj) {
 float a,b,c,d;
 int e,f,g;
 float time=func_00239030();
 if(obj->x20<time) {
  func_0020EC78(&a,&b,&c,&d,&e,&f,&g);
  a*=obj->x18; b*=obj->x18; d*=obj->x18;
  if(a<obj->x0) a=obj->x0;
  if(b<obj->x8) b=obj->x8;
  if(d<obj->x10) d=obj->x10;
  obj->x28=3.0f;
  func_0020ECD0(e, f, g, a, b, c, d);
  func_00216F48(obj->x28);
 } else if(time<obj->x24) {
  func_0020EC78(&a,&b,&c,&d,&e,&f,&g);
  a*=obj->x1C; b*=obj->x1C; d*=obj->x1C;
  if(a>obj->x4) a=obj->x4;
  if(b>obj->xC) b=obj->xC;
  if(d>obj->x14) d=obj->x14;
  obj->x28=1.0f;
  func_0020ECD0(e, f, g, a, b, c, d);
  func_00216F48(obj->x28);
 }
}
