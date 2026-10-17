/* UNIT 0x00125A28 0x00125CD8 */
#include "rider_event.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Part { char pad0[0x234]; char x234[0x18]; char x24C; } Part;
typedef struct Obj { char pad0[8]; int x8; char padC[0xC]; int x18,x1C; char pad20[0x48]; char x68[0x70]; float xD8; char padDC[4]; Vec xE0; char padF0[0x40]; Part *x130; char pad134[0xC]; void *x140; char *x144; } Obj;
extern int func_00122D10(Obj *,float);
extern void func_00123760(Obj *,void *);
extern void func_00123788(Obj *,Vec *);
extern void func_001238B8(Obj *);
extern void func_00123F60(Obj *,int);
extern void func_00125CD8(Obj *,Vec *);
extern void func_00126B20(Obj *,int);
extern float func_001385A8(void *);
extern void func_001ACAE0(void *,float);
extern float func_001ACBA0(void *);

extern float func_00235EA8(float,float,float,float);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline float Min(float a,float b) { float r; __asm__ volatile("min.s %0,%1,%2" : "=&f"(r) : "f"(a),"f"(b)); return r; }
void func_00125A28(Obj *obj) {
 Vec tmp;
 float distance;
 void *target;
 Vec *vec;
 int present;
 if (func_00122D10(obj,200.0f)==0) {
 if (obj->x8!=0) { func_00126B20(obj,0); obj->x130->x24C=1; obj->x144=0; }
 } else {
 obj->x130->x24C=0;
 if (obj->x1C>=61) { if (obj->x8!=3) func_00126B20(obj,3); }
 else {
 target=obj->x140;
 distance=obj->xD8;
 COPY_VECTOR(tmp,&obj->xE0);
 func_00123760(obj,target);
 if (obj->x144) {
 distance += distance*(1.0f-Min(1.0f,func_001385A8(obj->x144+0x40C0)));
 }
 if (obj->x144 && distance<10000.0f) {
 if ((unsigned)(obj->x8-2)>=2) { func_00126B20(obj,2); SubmitRiderEvent(obj->x130->x234,obj->x130); }
 } else if (obj->x8==2 && obj->x18>=181) { func_00126B20(obj,3); }
 }
 if (obj->x8==0) func_00126B20(obj,1);
 switch(obj->x8) {
 case 0: case 1:
 func_001ACAE0(obj->x68,func_00235EA8(func_001ACBA0(obj->x68),0.5f,0.1f,0.0001f)); func_00123F60(obj,0); break;
 case 2: func_00125CD8(obj,&tmp); func_001238B8(obj); break;
 case 3:
 present=(int)obj->x140;
 COPY_VECTOR(tmp,&obj->xE0);
 if(present) { func_00123788(obj,&obj->xE0); if(obj->x18>=121) func_00126B20(obj,1); }
 func_001ACAE0(obj->x68,func_00235EA8(func_001ACBA0(obj->x68),0.5f,0.1f,0.0001f)); func_00123F60(obj,0); break;
 }
 }
}
