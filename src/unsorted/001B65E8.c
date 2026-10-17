/* UNIT 0x001B65E8 0x001B6668 */
struct Node { unsigned int x0; char pad4[6]; unsigned short xA; char padC[4]; struct Node *x10; };
struct Obj { char pad[0x7930]; void *x7930; struct Node *x7934; char pad7938[8]; int x7940; };
extern void *func_00208FB8(void);
extern void func_00218278(void *, struct Node *);
void func_001B65E8(struct Obj *obj, struct Node *node) {
    struct Node *child;
    obj->x7940 = 0;
    obj->x7930 = 0;
    if ((node->x0 & 0x3F) == 6) {
        child = node->x10;
        if ((child->x0 & 0x3F) == 1) {
            obj->x7934 = child;
            obj->x7930 = func_00208FB8();
            func_00218278(obj->x7930, child);
            child->xA |= 4;
        }
    }
}
