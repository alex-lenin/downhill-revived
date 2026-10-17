/* UNIT 0x001675D8 0x00167690 */
typedef struct Obj {
 char pad0[1280];
 int x500;
 char pad504[4];
 float x508;
 float x50C;
 float x510;
 float x514;
 float x518;
 float x51C;
 float x520;
 float x524;
 float x528;
 float x52C;
 float x530;
 float x534;
 float x538;
 float x53C;
} Obj;
extern void func_0020EC78(float *,float *,float *,float *,int *,int *,int *);
#include "scene_effects.h"
void func_001675D8(Obj *p) {
 int out[3];
 func_0020EC78(&p->x524,&p->x528,&p->x52C,&p->x530,&out[0],&out[1],&out[2]);
 p->x534=(float)out[0]; p->x538=(float)out[1]; p->x53C=(float)out[2];
 if(p->x500) func_0020ECD0((int)p->x514, (int)p->x518, (int)p->x51C, p->x508, p->x528, p->x50C, p->x510);
}
