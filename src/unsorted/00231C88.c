/* UNIT 0x00231C88 0x00231CC0 */
#include "impulse.h"
typedef Vector4 Vec;

typedef ImpulseBody Obj;

void func_00231C88(Obj *obj, Vec *delta) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(obj->linearImpulse) : "r"(&obj->linearImpulse), "r"(delta));
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(obj->velocityDelta) : "r"(&obj->linearImpulse), "f"(obj->inverseMass) : "$2");
}
