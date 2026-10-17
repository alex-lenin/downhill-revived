/* UNIT 0x001595E0 0x00159690 */
typedef struct Node { int flags; char pad[0xC]; struct Node *x10; } Node;
typedef struct { char pad[0x2860]; int entries[49]; int count; } Obj;
extern int func_00209108(void);
extern void func_002182A8(int, int);
extern void func_00218278(int, Node *);
void func_001595E0(Obj *obj, Node *node) {
    Node *child;
    int entries;
    if ((node->flags & 0x3F) == 6) {
        child = node->x10;
        if ((child->flags & 0x3F) == 1) {
            obj->entries[obj->count] = func_00209108();
            func_002182A8(obj->entries[obj->count], 0);
            func_00218278(obj->entries[obj->count], child);
            obj->count++;
        }
    }
}
