/* UNIT 0x00230EC0 0x00230EF8 */
typedef struct { int x0, x4, x8; } Obj;
extern void func_00231058(int);
Obj *func_00230EC0(Obj *obj, int value) {
    obj->x4 = 0;
    obj->x8 = 0;
    func_00231058(value);
    return obj;
}
