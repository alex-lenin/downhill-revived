/* UNIT 0x001333C0 0x00133408 */
#include "impulse.h"
typedef unsigned int u128 __attribute__((mode(TI)));

u128 *func_001333C0(u128 *out,int unused,u128 *in,int arg,void *p) {
 func_00231CC0((ImpulseBody *)(p), (Vector4 *)(in), (Vector4 *)(arg));
 *out = *in;
 return out;
}
