/* UNIT 0x001A9268 0x001A93B8 */
typedef struct Entry { float x0,x4,x8,xC,x10; char pad[5]; signed char x19; char tail[6]; } Entry;
typedef struct Obj { char p0[0x30]; int x30; char p34[0x13]; signed char x47; char p48[0xC]; int x54; char p58[0x968]; int x9C0; char p9C4[0xC]; Entry entries[1]; } Obj;
#include "rider.h"
extern Rider *D_0037ED14[];
extern int func_001A8918(void *, float);
extern int func_001A8830(void *, int *);
extern void func_001A93B8(Obj *, Entry *, int);
extern void func_001A9508(Obj *);
void func_001A9268(Obj *obj) {
 int local, index; Entry *entry; Obj *base; float time, limit;
 time = D_0037ED14[0]->bodyNode->position.z;
 if (func_001A8918((char *)obj + (obj->x54 * 20 + 0x5C), time)) {
  index = func_001A8830((char *)obj + (obj->x54 * 20 + 0x5C), &local);
  obj->x9C0 = index;
  entry = &obj->entries[index];
  if (time < entry->xC && entry->x10 < time) {
   obj->x54++;
   func_001A93B8(obj, entry, local);
   obj->x30 = 10; obj->x47 = 1; return;
  }
  if (obj->x47) {
   base = (Obj *)((char *)obj + (obj->x9C0 << 5));
   if (time < base->entries[0].x10) goto stop;
   if (base->entries[0].x19 == 2) { limit = base->entries[0].x8; goto test; }
  }
 } else if (obj->x47) {
  limit = obj->entries[obj->x9C0].x10;
 test:
  if (time < limit) {
 stop:
   func_001A9508(obj); obj->x47 = 0;
  }
 }
}
