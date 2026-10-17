/* UNIT 0x001873F8 0x00187438 */
struct Obj { char pad0[0x418]; int x418; char pad41C[0xE4]; int x500[64]; };
void func_001873F8(struct Obj *p) {
 int count; int *q;
 p->x418=0;
 count=63; q=&p->x500[63];
 do { *q=0; count--; q--; } while(count>=0);
}
