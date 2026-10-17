/* UNIT 0x00196198 0x001961D8 */
struct Entry { char pad[8]; int *x8; char rest[8]; };
struct Obj { char pad[24]; struct Entry entries[152]; char rest[8]; int xc00; };
int func_00196198(struct Obj *obj) {
 int total = obj->xc00, count = 0, i = 0;
 struct Entry *entry;
 if (total > count) {
  entry = obj->entries;
  for (; i < total; entry++, i++) {
   if (*entry->x8) count++;
  }
 }
 return count;
}
