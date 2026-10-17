/* UNIT 0x00201B00 0x00201B60 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Packet { union { long x0; int id; } key; int x8, xC; u128 x10; short x20, x22; int x24, flags; };
extern void func_00201E90(int, int, void *);
void func_00201B00(int id, long key, u128 value, int x8, int xC, int x20, int x22) {
 struct Packet p;
 p.flags = 0;
 if (!id) p.key.x0 = key;
 else { p.key.id = id; p.flags = 1; }
 p.x10 = value; p.x8 = x8; p.xC = xC; p.x20 = x20; p.x22 = x22;
 p.flags |= 0x3C;
 func_00201E90(0x5C, 0x30, &p);
}
