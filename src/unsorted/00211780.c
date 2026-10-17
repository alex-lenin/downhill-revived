/* UNIT 0x00211780 0x002117F8 */
#include "spatial.h"
typedef SpatialHit Obj;
Obj *func_00211780(Obj *o,int count,Obj *prev) { Obj *best=0; if(count) do { if((!best||best->x10.w>o->x10.w)&&(!prev||prev->x10.w<o->x10.w||(prev->x10.w==o->x10.w&&(unsigned int)prev<(unsigned int)o))) best=o; --count; ++o; } while(count); return best; }
