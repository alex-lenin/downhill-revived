/* UNIT 0x0013C400 0x0013C5E0 */
#include "animation.h"
typedef struct Entry { char pad[6]; unsigned char x6; char rest[9]; } Entry;
typedef struct VEntry { short delta; short pad; void (*fn)(void *); } VEntry;
typedef struct Child { int x0; char pad4[0x14]; Entry entries[64]; int x418; char pad41c[0xdc]; VEntry *x4f8; } Child;
typedef struct Obj { char pad[0x500]; Child *x500; char pad504[0x1c]; int x520,x524,x528; } Obj;
void func_0013C400(Obj *obj,int state) {
 AnimationHandle sound; Child *child; Entry *entry; int count,active,one;
 if(state==1) {
  child=obj->x500; active=0;
  if(!child) active=1;
  else { if(child->x418>0) { count=child->x418; one=1; entry=child->entries; do { if(entry->x6) active=one; entry++; } while(--count); } }
  if(!active) return;
  if(child) { child->x0=0; child=obj->x500; child->x4f8[1].fn((char *)child+child->x4f8[1].delta); }
  if(obj->x520>=0) {
   sound.x8=0; func_00203590(&sound,0x2588,0,obj->x520);
   func_00204080(&sound,1); func_00202E38(&sound,1,7,200.0f);
   func_00203590(&sound,0x259d,0,0); func_00204080(&sound,1); func_00202E38(&sound,1,7,200.0f); obj->x524=1;
  }
 } else {
  child=obj->x500;
  if(child) { child->x4f8[2].fn((char *)child+child->x4f8[2].delta); obj->x500->x0=-1; }
  if(obj->x520>=0) {
   sound.x8=0; func_00203590(&sound,0x2588,0,obj->x520);
   func_00204080(&sound,0); func_00202E38(&sound,1,7,200.0f);
   func_00203590(&sound,0x259d,0,0); func_00204080(&sound,0); func_00202E38(&sound,1,7,200.0f); obj->x524=2;
  }
 }
 obj->x528=state;
}
