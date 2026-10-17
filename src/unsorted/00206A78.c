/* UNIT 0x00206A78 0x00206AD8 */
typedef struct Node { char pad[0x1C]; int flags; } Node;
typedef struct Obj { int x0; Node *x4; int x8, xC, x10; } Obj;
int func_00206A78(Obj *p) {
    int flags = p->x4->flags;
    int result;
    if (flags & 2) return 0;
    if (p->x8 != 0 && !(flags & 4)) return 1;
    if (p->x10 != 0 && !(p->x4->flags & 8)) return 1;
    return 0;
}
