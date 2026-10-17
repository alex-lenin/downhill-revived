/* UNIT 0x00113330 0x00113398 */
#include "rider.h"
typedef struct Values { float x0, x4, x8; } Values;
typedef struct Owner { char pad0[0x2E94]; Values x2E94; } Owner;
typedef struct Obj { Owner *x0; } Obj;

void func_00113330(Obj *obj, int arg1, Values *vec) {
 Values *values = &obj->x0->x2E94;
 if (values->x4 / values->x8 > 0.8f) {
  float length;
  float x = vec->x0, y = vec->x4, z = vec->x8;
  __asm__ ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0" : "=&f"(length) : "f"(x), "f"(y), "f"(z));
  func_00113398((RiderPhysics *)(obj), (Rider *)(arg1), (const Vector4 *)(vec), length);
 }
}
