/* UNIT 0x00133718 0x00133760 */
#include "impulse.h"
typedef unsigned int Vector __attribute__((mode(TI)));

void *func_00133718(char *obj,Vector *input) {
 Vector vector=*input;
 char *position;
 func_00231C88((ImpulseBody *)(obj), (Vector4 *)(&vector));
 position=obj+0x250;
 __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x250(%2)" : : "r"(position),"r"(&vector),"r"(obj) : "memory");
 return position;
}
