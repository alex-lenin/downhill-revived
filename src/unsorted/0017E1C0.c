/* UNIT 0x0017E1C0 0x0017E300 */
typedef struct Obj {
    char pad0[0x1B0];
    int x1B0;
    int x1B4;
    char pad1B8[0x3A4 - 0x1B8];
    int x3A4;
    int x3A8;
    char pad3AC[0x429 - 0x3AC];
    unsigned char x429;
    unsigned char x42A;
} Obj;

extern void func_0017EB00(Obj *obj, int id, int value);
extern int func_00220AE0(int axis, int value);

void func_0017E1C0(Obj *obj, int value)
{
    int v;

    func_0017EB00(obj, 7, value);
    func_0017EB00(obj, 8, value);
    obj->x429 = 0;
    obj->x42A = 0;
    if (obj->x3A4 != 0 || obj->x3A8 != 0) {
        return;
    }
    if (obj->x1B0 == 0 && obj->x1B4 == 0) {
        v = func_00220AE0(2, value);
        obj->x1B0 = v;
        obj->x1B4 = 0;
        if (v < 0) {
            obj->x1B4 = v;
            obj->x1B0 = 0;
        }
        if (obj->x1B0 != 0) {
            if (obj->x1B0 < 127) {
                obj->x1B0 = obj->x1B0 * obj->x1B0 / 127;
            } else {
                obj->x1B0 = 127;
            }
        }
        if (obj->x1B4 != 0) {
            if (obj->x1B4 < 127) {
                obj->x1B4 = obj->x1B4 * obj->x1B4 / 127;
            } else {
                obj->x1B4 = 127;
            }
        }
        obj->x42A = obj->x1B4 != 0;
        obj->x429 = obj->x1B0 != 0;
    } else {
        if (obj->x1B0 != 0) {
            obj->x1B0 = 1;
        }
        if (obj->x1B4 != 0) {
            obj->x1B4 = 1;
        }
    }
}
