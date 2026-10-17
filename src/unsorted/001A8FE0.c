/* UNIT 0x001A8FE0 0x001A9038 */
struct Entry { short x0; signed char x2, x3, x4, x5; char pad[2]; };
struct A { char pad0[0xc]; struct Entry *xC; char pad10[0x14]; int x24; };
extern void func_002202B8(int, int);
extern void func_002202D0(int, int, int, int, int);
void func_001A8FE0(struct A *arg0) {
 struct Entry *entry = arg0->xC + arg0->x24;
 func_002202B8(0, entry->x0);
 func_002202D0(0, entry->x2, entry->x3, entry->x4, entry->x5);
}
