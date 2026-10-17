/* UNIT 0x001AAC00 0x001AAC58 */
#include "impulse.h"

struct A {
 char pad[0x20];
 char *x20;
 int func_001AA318();
 void func_001AAC00(int);
};
void A::func_001AAC00(int arg1) {
 if (x20 || func_001AA318()) {
  func_00231C88((ImpulseBody *)(x20 + 0x40), (Vector4 *)(arg1));
 }
}
