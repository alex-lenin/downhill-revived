/* UNIT 0x00152088 0x001520E0 */
struct Packed { unsigned int low:7; unsigned int flags:11; unsigned int high:14; };
struct Item { int x0; unsigned int x4; struct Packed *x8; char pad[0x24]; };
extern struct Item D_003830B0[50];
void func_00152088(void) {
 struct Item *item = D_003830B0;
 do {
  struct Packed *p = item->x8;
  if (p) p->flags = item->x4;
  item++;
 } while ((int)item < (int)(D_003830B0 + 50));
}
