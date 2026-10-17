/* UNIT 0x00144520 0x00144688 */
#include "motion_interpolation.h"
#if defined(__mips__)
#define CallShared001DD218_0 func_001DD218
#else
#define CallShared001DD218_0 Forward001DD218_0
#endif
typedef struct Vector { float x,y,z,w; } Vector;
typedef struct Base { unsigned int *x0; char pad4[0xC]; Vector x10; char pad20[0x20]; Vector x40; Vector x50; char pad60[0x40]; float xA0,xA4,xA8; } Base;
typedef struct Settings { char pad[0x794]; float x794,x798; } Settings;
typedef struct Obj { Base *x0; Settings *x4; char pad8[8]; Vector x10; char pad20[16]; Vector x30,x40; char pad50[0x30]; int x80,x84,x88; } Obj;
extern char D_002AF2C0[];
extern int func_0011C0D0(void *,void *,int,float);
extern void func_001DD218(Vector *,float,float,Vector *,void *);
extern void func_001DCE58(Vector *);
extern float func_00222610(Vector *,int);
extern float func_00222660(Vector *,int);
extern float func_00236130(float,float,float,float);
extern void func_00221F50(Vector *,float *);
extern void func_00211280(Base *);
#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00144520(Obj *obj) {
 Vector vec;
 Vector *pos;
 int *component;
 float value;
 if(func_0011C0D0(D_002AF2C0,&obj->x0->x10,0,7000.0f)) {
  pos=&obj->x10;
  component=&obj->x80;
  CallShared001DD218_0(pos,obj->x4->x794,obj->x4->x798,&vec,component);
  COPY_VECTOR(obj->x40,&vec);
  func_001DCE58(pos);
  if((*obj->x0->x0 & 0xFFFC0000)==0x13D80000) {
   value=func_00222610(&obj->x0->x10,component[2]);
   obj->x0->xA4=0;
   obj->x0->xA8=func_00236130(obj->x0->xA8,value,0.002f,0.001f);
   value=func_00222660(&obj->x0->x10,component[2]);
   obj->x0->xA0=func_00236130(obj->x0->xA0,value,0.01f,0.001f);
   func_00221F50(&obj->x0->x40,&obj->x0->xA0);
   COPY_VECTOR(obj->x30,&obj->x0->x50);
  }
  COPY_VECTOR(obj->x0->x10,pos);
  func_00211280(obj->x0);
 }
}
