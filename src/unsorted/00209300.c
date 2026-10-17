/* UNIT 0x00209300 0x002093C8 */
extern void func_00221D58(void *,void *);
extern void func_00221BE0(void *,void *,void *);
extern void func_00221C20(void *,void *,float);
#include "vector.h"
void func_00209300(float *obj,float *out) {
 float a[16] __attribute__((aligned(16)));
 float b[16] __attribute__((aligned(16)));
 float c[16] __attribute__((aligned(16)));
 float d[16] __attribute__((aligned(16)));
 func_00221D58(a,obj+16);
 func_00221D58(b,obj+48);
 func_00221BE0(c,a,b);
 func_00221C20(c,c,1.0f/obj[44]);
 func_002219A8((Matrix4 *)d,(Matrix4 *)c,(Matrix4 *)b);
 out[0]=-d[6]; out[1]=-d[8]; out[2]=-d[1]; out[3]=0.0f;
}
