/* UNIT 0x0017CCB0 0x0017CD20 */
typedef struct Node { int flags; char pad4[0x1c]; struct Node *x20; } Node;
typedef struct Obj { char pad[0x18]; Node *x18; Node *x1c; int x20; } Obj;
void func_0017CCB0(Obj *obj) {
 Node *node; int flags;
 node = obj->x1c;
 if (node) {
  node->flags = (node->flags & ~0x3f) | 3;
  node = obj->x18;
  node->flags &= 0xfffc007f;
  if (obj->x20) {
   node = obj->x18->x20;
   flags = node->flags;
   if ((flags & 0x3f) == 9) node->flags = (flags & ~0x3f) | 4;
  }
 }
}
