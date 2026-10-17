/* UNIT 0x001926E0 0x00192880 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundHeightFirst
#endif
typedef struct {float v[4];} Vec;
typedef struct {char pad[0x10];Vec pos;char pad20[0x20];float mat[16];char pad80[0x20];float xA0,xA4,xA8;} State;
typedef struct {char pad[8];State *x8;char padc[0x44];Vec x50;float x60,x64;} Obj;
extern float func_00235F38(float,float,float,float);
extern float func_00235EA8(float,float,float,float);
extern void func_00221F50(void *,void *);
#if defined(__mips__)
extern float func_00211980(void *,void *,float,int);
#endif
extern void func_00211280(State *);
void func_001926E0(Obj *obj) {
 Vec v; State *state; float speed,height,z,target;
 obj->x8->xA0+=0.015f;
 if(obj->x8->xA0>1.3f) obj->x8->xA0=1.3f;
 obj->x8->xA4=func_00235F38(obj->x8->xA4,0.7853981852531433f,0.03f,0.001f);
 obj->x8->xA8+=0.003f;
 func_00221F50(obj->x8->mat,&obj->x8->xA0);
 speed=func_00235EA8(obj->x60,obj->x64,0.05f,0.001f);
 obj->x60=speed;
 state=obj->x8;
 v.v[0]=0.0f; v.v[1]=speed; v.v[2]=0.0f;
 __asm__ volatile("lqc2 $vf11, %0\n\tlqc2 $vf12, 0x0(%1)\n\tlqc2 $vf13, 0x10(%1)\n\tlqc2 $vf14, 0x20(%1)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : : "m"(v),"r"(state->mat) : "memory");
 __asm__ volatile("lqc2 $vf11, 0x0(%0)\n\tlqc2 $vf12, %1\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0x10(%2)" : : "r"(&state->pos),"m"(v),"r"(state) : "memory");
 height=CallGroundProbe(&obj->x8->pos,&obj->x50,15.0f,0);
 z=obj->x8->pos.v[2];
 target=z-height;
 if(height<5.0f) obj->x8->pos.v[2]=func_00235EA8(z,target+5.0f,0.03f,0.001f);
 func_00211280(obj->x8);
}
