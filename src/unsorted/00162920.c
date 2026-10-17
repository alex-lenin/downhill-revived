/* UNIT 0x00162920 0x001629D0 */
typedef struct { char pad[0x276C]; unsigned short type; } Child;
typedef struct { char pad[0x7984]; int x7984; int x7988; char pad2[0xC0]; Child *x7A4C; Child *x7A50; Child *x7A54; } Obj;
extern void func_0022DDD8(Child *);
extern void func_002399F8(int);
void func_00162920(Obj *obj) {
    Child *a = obj->x7A4C;
    Child *b = obj->x7A50;
    Child *c = obj->x7A54;
    if (a && a->type == 50) {
        func_0022DDD8(a);
        obj->x7A4C = 0;
    }
    if (b && b->type == 38) {
        func_0022DDD8(b);
        obj->x7A50 = 0;
    }
    if (c && c->type == 39) {
        func_0022DDD8(c);
        obj->x7A54 = 0;
    }
    if (obj->x7984) func_002399F8(obj->x7988);
}
