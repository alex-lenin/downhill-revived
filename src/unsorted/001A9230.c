/* UNIT 0x001A9230 0x001A9268 */
typedef struct Obj { char pad[0x30]; int x30; } Obj;
extern void func_001A9268(Obj *);
void func_001A9230(Obj *obj) {
    if (obj->x30 <= 0) {
        func_001A9268(obj);
    } else {
        obj->x30--;
    }
}
