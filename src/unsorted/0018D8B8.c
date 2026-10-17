/* UNIT 0x0018D8B8 0x0018D8F8 */
struct Entry { char pad[6]; unsigned char x6; char rest[9]; };
struct Obj { char pad[24]; struct Entry entries[64]; int count; };
int func_0018D8B8(struct Obj *obj) {
 int count = 0, total = obj->count, remaining;
 struct Entry *entry;
 if (total > 0) {
  entry = obj->entries; remaining = total;
  do { count += entry->x6 != 0; entry++; remaining--; } while (remaining != 0);
 }
 return count;
}
