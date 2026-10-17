/* UNIT 0x001A95E8 0x001A9640 */
struct Entry { char pad[0x358]; int x358; char tail[0x24]; };
struct A { char pad[0x34]; int x34; };
extern struct Entry D_0037EA60[];
extern void func_0014D208(void *, int);
void func_001A95E8(struct A *arg0, int index) {
 struct Entry *entry = &D_0037EA60[index];
 if (arg0->x34 && entry->x358 == 0) {
  func_0014D208(entry, 0);
  arg0->x34 = 0;
 }
}
