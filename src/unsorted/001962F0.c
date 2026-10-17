/* UNIT 0x001962F0 0x00196388 */
struct Item { char pad[20]; };
struct State { char pad0[24]; struct Item items[150]; int xBD0[6]; char padBE8[0x18]; int xC00; };
extern void func_00196558(struct Item *);
void func_001962F0(struct State *self) {
 int i=0; int *base=self->xBD0;
 do {
  int j; int *p;
  func_00196558(&self->items[i]); j=5; p=base+5;
  do { *p=0; --j; --p; } while(j>=0);
  ++i;
 } while(i<150);
 self->xC00=0;
}
