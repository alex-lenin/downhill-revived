/* UNIT 0x00125178 0x00125598 .rodata=0x288DD0:0x288DF0 */
#include "rider_ai.h"
#include "rider_event.h"
struct Vec { float x,y,z,w; } __attribute__((aligned(16)));
class PartBase {
public:
 virtual void v0();
 virtual void v1();
 virtual void v2();
 virtual char *v3();
};
class Part : public PartBase {
public:
 char pad4[0x230];
 char x234[0x18];
 char x24C;
};
struct Target { char pad0[0x792C]; char *x792C; };
struct Obj {
 char pad0[8]; int x8; char padC[8]; int x14; char pad18[4]; int x1C,x20;
 char pad24[0x44]; char x68[0x68]; int xD0; char padD4[4]; float xD8; char padDC[4]; Vec xE0;
 char padF0[0x40]; Part *x130; char pad134[4]; Target *x138; char pad13C[4]; void *x140; Target *x144;
};
extern "C" {
int func_00122D10(Obj *,float);
void func_00123760(Obj *,void *);
void func_00123788(Obj *,void *);
void func_001238B8(Obj *);
void func_00123AD0(Obj *,Vec *,int);
void func_00123DB0(Obj *,void *);
void func_00123F60(Obj *,int);
void func_001246D0(Obj *);

void func_00125598(Obj *,Vec *);
void func_00126B20(Obj *,int);
float func_001385A8(void *);
void func_001ACAE0(void *,float);
float func_001ACBA0(void *);
void func_001BD890(void *,Part *);

float func_00235EA8(float,float,float,float);
}
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline float Min(float a,float b) { float r; __asm__ volatile("min.s %0,%1,%2" : "=&f"(r) : "f"(a),"f"(b)); return r; }
extern "C" void func_00125178(Obj *obj) {
 Vec tmp;
 float distance;
 void *target;
 if (func_00122D10(obj,200.0f)==0) {
  if (obj->x8!=0) { func_00126B20(obj,0); obj->x130->x24C=1; obj->x144=0; func_001246D0(obj); }
  return;
 }
 obj->x130->x24C=0;
 if (obj->x1C>=61 || obj->x20>=61) {
  if (obj->x8!=1) func_00126B20(obj,1);
 } else {
  target=obj->x140;
  distance=obj->xD8;
  COPY_VECTOR(tmp,&obj->xE0);
  func_00123760(obj,target);
  if (obj->x144) {
   distance += distance*(1.0f-Min(1.0f,func_001385A8((char *)obj->x144+0x40C0)));
   if (obj->x144) {
    if (distance<9.0f) {
     if (obj->x8!=5) { func_00126B20(obj,5); SubmitRiderEvent(obj->x130->x234,obj->x130); }
     goto done;
    }
    if (distance<144.0f) {
     if (func_001385A8((char *)obj->x144+0x40C0)<0.1f) {
      if (obj->x8!=3) func_00126B20(obj,3);
     } else {
      if (obj->x8!=4) { func_00126B20(obj,4); SubmitRiderEvent(obj->x130->x234,obj->x130); }
     }
     obj->xD0=0;
     goto done;
    }
   }
  }
  if ((unsigned)(obj->x8-1)>=2) {
   if (obj->x144) {
    if (distance>576.0f) {
     if (obj->x14) func_00126B20(obj,1); else func_00126B20(obj,2);
    } else {
     func_00126B20(obj,4); SubmitRiderEvent(obj->x130->x234,obj->x130);
     obj->xD0=0;
    }
   } else {
    if (obj->x14) func_00126B20(obj,1); else func_00126B20(obj,2);
   }
  }
 done:
  func_001BD890(obj->x130->x234,obj->x130);
 }
 if (obj->x8==0) func_00126B20(obj,2);
 switch (obj->x8) {
 case 0:
  break;
 case 1:
  if (obj->x144) func_00123788(obj,obj->x144->x792C+0x10);
  else if (obj->x138) func_00123788(obj,obj->x138->x792C+0x10);
  func_001ACAE0(obj->x68,func_00235EA8(func_001ACBA0(obj->x68),0.5f,0.03f,0.0001f));
  func_00123F60(obj,0);
  func_00123DB0(obj,obj->x130->v3()+0x40);
  break;
 case 2:
  func_00125598(obj,&tmp);
  func_001238B8(obj);
  break;
 case 4:
  func_00123AD0(obj,&tmp,0);
  func_001238B8(obj);
  break;
 case 3:
  func_00123AD0(obj,&tmp,1);
  func_001238B8(obj);
  break;
 case 5:
  UpdateRiderAiLegacy(obj,&tmp);
  func_001ACAE0(obj->x68,func_00235EA8(func_001ACBA0(obj->x68),0.5f,0.1f,0.0001f));
  break;
 }
}
