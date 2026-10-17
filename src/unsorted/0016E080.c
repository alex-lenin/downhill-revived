/* UNIT 0x0016E080 0x0016E110 */
struct Obj { char pad[0x6BC]; unsigned char x6BC,x6BD; };
extern void func_0016D488(struct Obj *, void *);
extern unsigned char func_0016D8C0(struct Obj *, void *);
extern void func_0016EE40(struct Obj *);
void func_0016E080(struct Obj *obj) {
    void *first = (char *)obj + 0x250;
    void *second = (char *)obj + 0x480;
    func_0016D488(obj, first);
    func_0016D488(obj, second);
    if (obj->x6BC == 0) obj->x6BC = func_0016D8C0(obj, first);
    else if (obj->x6BD == 0) obj->x6BD = func_0016D8C0(obj, second);
    else func_0016EE40(obj);
}
