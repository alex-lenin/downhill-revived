/* UNIT 0x001AEB20 0x001AEB88 */
#include "camera.h"
#include "rider.h"

typedef RiderComponent Parent;
typedef RiderCamera Other;
typedef RiderCameraRig Obj;
extern void func_002185C0(void *,int);
void func_001AEB20(Obj *obj,Other *other,Vector4 *vector) {
 float result[4] __attribute__((aligned(16)));
 char *matrix = (char *)&other->xA10->rider->bodyNode->matrix;
 __asm__ volatile (
 "lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%2)"
 : : "r"(vector),"r"(matrix),"r"(result) : "memory");
 {
 char *target=(char *)obj->xA0;
 char *position=target+0x10;
 __asm__ volatile (
 "lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,16(%2)"
 : : "r"(position),"r"(result),"r"(target) : "memory");
 }
 func_002185C0(obj->xA0,0);
}
