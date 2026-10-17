/* UNIT 0x0019E9E8 0x0019EA38 */
struct Item { unsigned int flags; };
struct Obj { char pad[0x1a0]; struct Item *items[35]; };
void func_0019E9E8(struct Obj *o) {
 int i; struct Item **p=o->items;
 for(i=0;i<35;i++,p++) {
  if(*p) (*p)->flags=((*p)->flags & 0xfffc007fU) | ((i & 0x7ff)<<7);
 }
}
