/* UNIT 0x001DBCE8 0x001DBE38 */
#include "tween.h"
typedef struct Vector { float x,y,z,w; } Vector;
typedef struct Obj { char *x0; int x4; float x8; } Obj;
extern void func_001ACFA0(void *,int);
extern int func_0012D300(void *);
extern void func_00202E38(Vector *,int,int,float);
extern void func_00203E10(Vector *,float);

extern void func_001ACF38(void *,int,float);
extern void func_00136030(void *,int);
void func_001DBCE8(Obj *obj) {
 Vector local; char *base, *anim; Vector *first,*second; int offset;
 base=obj->x0; anim=base+0x4580; first=(Vector *)(base+0x1E40);
 func_001ACFA0(anim,0);
 *(int *)(base+0x48B0)=0;
 offset=(func_0012D300(obj->x0)+19)*16;
 first=(Vector *)((char *)first+(offset+20));
 second=(Vector *)(base+offset);
 func_00202E38(first,1,1,obj->x8);
 local=*first; func_00203E10(&local,0.0f);
 func_00202E38(second,1,1,obj->x8);
 local=*second; func_00203E10(&local,0.0f);
 func_001B3170((RiderSecondaryBlend *)(base+0x3530), (int)obj->x8, 0.0f);
 func_001ACF38(anim,(int)(obj->x8*0.0625f),1.0f);
 func_00136030(base+0x40C0,1);
}
