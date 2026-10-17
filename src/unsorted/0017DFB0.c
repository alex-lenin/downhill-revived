/* UNIT 0x0017DFB0 0x0017E070 */
struct Obj {
    char pad0[0x1A0]; int x1A0; int x1A4;
    char pad1[0x394-0x1A8]; int x394; int x398;
    char pad2[0x425-0x39C]; unsigned char x425; unsigned char x426;
};
extern void func_0017EB00(struct Obj *, int, int);
extern int abs(int);
extern int func_00220AE0(int, int);
void func_0017DFB0(struct Obj *obj, int arg) {
    int value;
    func_0017EB00(obj, 3, arg);
    func_0017EB00(obj, 4, arg);
    obj->x425 = 0;
    obj->x426 = 0;
    if (!obj->x394 && !obj->x398 && !obj->x1A0 && !obj->x1A4) {
        value = -func_00220AE0(1, arg);
        obj->x1A4 = 0;
        obj->x1A0 = value;
        if (value < 0) {
            value = abs(value);
            obj->x1A0 = 0;
            obj->x1A4 = value;
        }
        obj->x425 = obj->x1A0 != 0;
        obj->x426 = obj->x1A4 != 0;
    }
}
