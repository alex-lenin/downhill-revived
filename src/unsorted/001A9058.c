/* UNIT 0x001A9058 0x001A9230 */
struct Settings { char pad[0x140]; float x140[100]; };
struct Obj { char p0[8]; int x8; char pC[4]; struct Settings *x10; char p14[16]; int x24; char p28[30]; signed char x46; char p47[0x981]; int x9C8; };
#include "rider.h"
struct Global { char pad[0x50130]; Rider *x50130; };
extern struct Global D_002AF2C0;
extern char D_004B1F40[],D_0028E888[];
extern void func_001A9568(struct Obj *);
extern void func_001A8FE0(struct Obj *);
extern void func_001A9598(struct Obj *,int);
extern void func_001B45F8(void *,int,int);
extern void func_0020F588(int,int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,void *);
int func_001A9058(struct Obj *o) {
 int seconds; float target;
 func_001A9568(o); func_001A8FE0(o); func_001A9598(o,0);
 if(o->x24-o->x9C8 >60) {
  seconds=o->x24/60; target=D_002AF2C0.x50130->bodyNode->position.z;
  if(seconds>0 && o->x10->x140[seconds] != target) {
   func_001B45F8(D_004B1F40,6,0); return 0;
  }
  o->x9C8=o->x24;
 }
 switch(o->x8) {
  case 0: o->x24++; break;
  case 2: if(o->x46) { func_001A8FE0(o); o->x24+=o->x46; } break;
 }
 if(o->x24<300) {
  func_0020F588(0,0,110,110,200,80); func_0020FB80(0,256,192,-91,-32); func_0020F9F0(0,0,240,200,D_0028E888);
 } else if(o->x24<360) {
  func_0020F588(0,0,110,110,200,360-o->x24); func_0020FB80(0,256,192,-91,-32); func_0020F9F0(0,0,240,200,D_0028E888);
 }
 return 1;
}
