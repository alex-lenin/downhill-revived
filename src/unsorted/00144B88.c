/* UNIT 0x00144B88 0x00144C18 */
struct Item { char pad[0x1c0]; };
struct Obj { int x0; char pad[12]; struct Item items[12]; char pad2[0x44]; int x1554; };
extern void func_001450A0(void *, int);
void func_00144B88(struct Obj *obj, int state) {
 struct Item *p; int i, mode;
 if (obj->x0 != state) {
  mode = 0;
  if (state != 0) mode = state == 1 ? 2 : 0;
  obj->x0 = state;
  for (i = 0; i < obj->x1554; i++) func_001450A0(&obj->items[i], mode);
 }
}
