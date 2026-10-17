/* UNIT 0x00208378 0x002083E8 */
typedef struct { int x0[4]; } Entry;
extern void func_00208338(Entry *);
extern void func_002083E8(Entry *);
Entry *func_00208378(Entry *obj) {
 Entry *entry = obj;
 int count = 11;
 do {
  --count;
  func_00208338(entry);
  ++entry;
 } while (count != -1);
 func_002083E8(obj);
 return obj;
}
