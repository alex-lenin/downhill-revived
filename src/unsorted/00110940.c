/* UNIT 0x00110940 0x001109B0 */
struct Sub { int x0, x4; char pad8[0xC4]; unsigned char xCC; };
struct Obj { char *x0; char pad4[0x2D0]; struct Sub x2D4; };
extern int func_001E6370(void *);
extern int func_00110D38(struct Obj *, int);
int func_00110940(struct Obj *obj) {
    struct Sub *sub = &obj->x2D4;
    int result = 0;
    unsigned long flags = sub->xCC;
    if (flags >> 7) {
        if (func_001E6370(obj->x0 + 0x6610)) {
            result = func_00110D38(obj, sub->x4) != 0;
        }
    }
    return result;
}
