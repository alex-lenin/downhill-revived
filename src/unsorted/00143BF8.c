/* UNIT 0x00143BF8 0x00143C50 */
struct Item { char pad[0xf80]; };
struct Obj { int x0; char pad4[12]; struct Item items[1]; };
extern void func_00143090(struct Item *);
void func_00143BF8(struct Obj *obj) {
 struct Item *item = obj->items;
 int count = 0;
 do { count--; func_00143090(item); item++; } while (count >= 0);
 obj->x0=0;
}
