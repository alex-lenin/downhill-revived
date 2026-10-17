/* UNIT 0x00143090 0x001430E8 */
struct Item { char pad[0x170]; };
struct Obj { int x0, x4; char pad8[8]; int x10; char pad14[0x5c]; struct Item items[10]; };
extern void func_00141CA0(struct Item *);
void func_00143090(struct Obj *obj) {
 struct Item *item = obj->items;
 int count = 9;
 obj->x4=0; obj->x10=0; obj->x0=0;
 do { count--; func_00141CA0(item); item++; } while (count >= 0);
}
