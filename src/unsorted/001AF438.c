/* UNIT 0x001AF438 0x001AF4F0 */
#include "camera.h"
struct Obj { float x0,x4,x8,xC,x10; unsigned char *x14; };
extern float func_00236088(float,float,float,float);

void func_001AF438(struct Obj *p) {
 unsigned char *r=p->x14; unsigned char *base; float x;
 if(r) {
  base=r+0x1E40;
  if(r[0x4414]==2 && r[0x4494]==r[0x4414]) {
   p->x8=0.03f; x=p->x0+p->x8;
   __asm__("min.s %0,%1,%2" : "=f"(x) : "f"(x),"f"(p->x10));
   p->x0=x;
  } else { p->x0=func_00236088(p->x0,0.0f,0.25f,0.1f); p->x10=0.3f; }
  func_001B3028((RiderCamera *)(base+0x16F0), 6, p->x0);
 }
}
