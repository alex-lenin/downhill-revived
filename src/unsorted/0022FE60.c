/* UNIT 0x0022FE60 0x00230058 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0021E3A8_31_func_0021E3A8 func_0021E3A8
#else
#define CallScalarBridge0021E3A8_31_func_0021E3A8 ScalarBridge0021E3A8_31
#endif
typedef float Vec[4] __attribute__((aligned(16)));
typedef struct { Vec rows[4]; } Mat;
typedef struct { char pad0[0x90]; float x90,x94,x98,x9C,xA0,xA4,xA8,xAC,xB0,xB4,xB8,xBC; } Obj;
extern float D_00736CC0[],D_00736CD0[],D_00736CE0[],D_00736CF0[],D_00736D00[],D_00736C80[];
extern Obj *func_0023DF70(int);
extern void func_00221F50(Mat *,Vec *);

#if defined(__mips__)
extern void func_0021E3A8(float);
#else

#endif

static inline void Transform(Vec *out,Mat *mat,Vec *v) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v),"r"(mat));
}
void func_0022FE60(void) {
 Vec angles; Mat mat; Vec up={0,0,0,0}; Vec result;
 int i=0,j; float *pitch,*yaw,*out;
 up[1]=1; up[3]=1;
 do {
 Obj *obj=func_0023DF70(i);
 i++;
 obj->x90=D_00736CC0[0]*0.0078125f;
 obj->xA0=D_00736CC0[1]*0.0078125f;
 obj->xB0=D_00736CC0[2]*0.0078125f;
 obj->x94=D_00736CD0[0]*0.0078125f;
 obj->xA4=D_00736CD0[1]*0.0078125f;
 obj->xB4=D_00736CD0[2]*0.0078125f;
 obj->x98=D_00736CE0[0]*0.0078125f;
 obj->xA8=D_00736CE0[1]*0.0078125f;
 obj->xB8=D_00736CE0[2]*0.0078125f;
 } while(i<6);
 for(j=0;j<3;j++) {
 angles[0]=D_00736D00[j]*0.017453292f; angles[1]=0; angles[2]=D_00736CF0[j]*0.017453292f;
 func_00221F50(&mat,&angles);
 Transform(&result,&mat,&up);

 D_00736C80[j]=result[0]*32; D_00736C80[j+4]=result[1]*32; D_00736C80[j+8]=result[2]*32;

 }
 CallScalarBridge0021E3A8_31_func_0021E3A8(D_00736CF0[1]);
}
