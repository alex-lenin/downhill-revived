/* UNIT 0x0016CB78 0x0016CC38 */
typedef struct Node { int flags; char pad4[6]; unsigned short xA; char padC[4]; struct Node *x10; } Node;
typedef struct Item { int x0; char x4; } Item;
typedef struct Obj { char pad0[0x28]; Item *items[4389]; int count; } Obj;
extern Item *func_00209108(void);
extern void func_00218278(Item *, Node *);
void func_0016CB78(Obj *obj, Node *node) {
    Node *child;
    int type;
    if ((node->flags & 63) == 6) {
        child = node->x10;
        if (child) {
            type = child->flags & 63;
            if (type == 1) {
                obj->items[obj->count] = func_00209108();
                obj->items[obj->count]->x4 = type;
                func_00218278(obj->items[obj->count], child);
                child->xA |= 4;
                obj->count++;
            }
        }
    }
}
