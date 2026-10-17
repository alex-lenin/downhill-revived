/* UNIT 0x0016F500 0x0016F6E0 */
#include "rider_queries.h"
#if defined(__mips__)
#define CallShared0011C0D0_0 func_0011C0D0
#else
#define CallShared0011C0D0_0 Forward0011C0D0_0
#endif
typedef struct {char pad[0x18]; float x18;} Target;
typedef struct {char pad[0x4484];float x4484,x4488;} State;
typedef struct {char pad[0x1f0];Target *x1f0;int x1f4;char *x1f8;State *x1fc;int x200,x204;char pad208[0x20];float x228;} Obj;
extern struct {char pad[0x5019c];float x5019c;} D_002AF2C0;
extern float D_0029C8B0[];
extern int func_0011C0D0(void *,void *,float,int);
extern char *func_0011B180(void *,void *,float *,int,float);
extern float func_001385A8(void *);
extern void func_0016F410(Obj *,int);
void func_0016F500(Obj *obj) {
 Target *target; char *result; float height, height2, oldHeight;
 target=obj->x1f0;
 if(target) {
  if(D_002AF2C0.x5019c+400.0f<target->x18) {
   if(obj->x204==2) func_0016F410(obj,1);
  } else {
   obj->x228=D_0029C8B0[0];
   if(CallShared0011C0D0_0(&D_002AF2C0,(char *)target+0x10,600.0f,0)) {
    result=func_0011B180(&D_002AF2C0,(char *)obj->x1f0+0x10,&obj->x228,0,600.0f);
    obj->x1f8=result;
    if(result && obj->x228<250000.0f) {
     if(func_001385A8(result+0x40c0)<0.3f) {
      height=obj->x1f0->x18;
      if(obj->x1fc->x4488<height) obj->x1fc->x4488=height;
     }
     if(obj->x204==1) func_0016F410(obj,2);
    }
   } else {
    oldHeight=obj->x1fc->x4484;
    height2=obj->x1f0->x18;
    if(height2-oldHeight>0.0f) {
     if(D_002AF2C0.x5019c+400.0f<height2) {
      func_0016F410(obj,1);
     } else if(obj->x204==2) func_0016F410(obj,1);
    }
   }
  }
 }
}
