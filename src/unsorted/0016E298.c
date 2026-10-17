/* UNIT 0x0016E298 0x0016E328 */
struct Obj { char pad[0x6BE]; unsigned char x6BE,x6BF; };
extern void func_0016D488(struct Obj *, void *);
extern unsigned char func_0016E678(struct Obj *, void *);
extern void func_0016EE40(struct Obj *);
void func_0016E298(struct Obj *obj) {
    if (obj->x6BE == 0) {
        void *first = (char *)obj + 0x250;
        func_0016D488(obj, first);
        obj->x6BE = func_0016E678(obj, first);
    }
    if (obj->x6BF == 0) {
        void *second = (char *)obj + 0x480;
        func_0016D488(obj, second);
        obj->x6BF = func_0016E678(obj, second);
    } else if (obj->x6BE != 0) func_0016EE40(obj);
}
