/* UNIT 0x0015B100 0x0015B1A0 */
#include "rider_geometry.h"
typedef struct { char pad0[0x190]; int x190; } Child;
typedef struct { char pad0[8]; char *x8; char padC[0x18]; Child *x24; char pad28[0x1B8]; short x1E0; } Obj;
extern void func_00159CE0(Obj *, int);
extern void func_00166948(Child *);
extern void func_00165560(Child *);
extern void func_00223348(void *, void *);

void func_0015B100(Obj *obj) {
 float zero[4] __attribute__((aligned(16)));
 Child *child = obj->x24;
 if (child) {
  if ((child->x190 ^ 1) == 0 || obj->x1E0 >= 121) {
   __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(zero) : "memory");
   func_00159CE0(obj, 9);
   func_00166948(obj->x24);
   obj->x24 = 0;
   func_00223348((char *)obj + 0x10, obj->x8 + 0x40);
   func_0015B910((GroundNpc *)(obj), (void *)(obj->x8 + 0x10), 1);
  } else func_00165560(child);
 }
}
