/* UNIT 0x00167690 0x00167700 */
struct Obj { char pad0[0x500]; int x500,x504; char pad508[0x38]; float x540,x544,x548,x54C,x550,x554,x558; };
#include "scene_effects.h"
extern void func_0018E468(struct Obj *);
void func_00167690(struct Obj *p) {
 func_0020ECD0((int)p->x550, (int)p->x554, (int)p->x558, p->x540, p->x544, p->x548, p->x54C);
 p->x500=0; p->x504=1; func_0018E468(p);
}
