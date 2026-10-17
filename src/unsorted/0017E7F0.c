/* UNIT 0x0017E7F0 0x0017E848 */
struct Obj { char pad0[0xd4]; int xD4; char padD8[0xf0]; int x1C8; char pad1CC[0x60]; int x22C; };
extern void func_0017EB00(struct Obj *, int, int);
extern void func_0017EEF8(struct Obj *, int, int, int);
void func_0017E7F0(struct Obj *obj, int value) {
    func_0017EB00(obj, 13, value);
    if (obj->x22C != 0 && obj->x1C8 == 0) {
        func_0017EEF8(obj, 0, obj->xD4, 20);
    }
}
