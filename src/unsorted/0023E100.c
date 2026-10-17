/* UNIT 0x0023E100 0x0023E168 */
struct Obj { int x0; char *x4; volatile int x8, xC; int x10; };
extern int func_0023E0F0(struct Obj *);
char *func_0023E100(struct Obj *obj) {
    int count, head, tail;
    if (func_0023E0F0(obj)) return 0;
    head = obj->x8;
    tail = obj->xC;
    count = obj->x10;
    return obj->x4 + ((head - tail + count) % count) * 0x85740;
}
