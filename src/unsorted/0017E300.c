/* UNIT 0x0017E300 0x0017E440 */
typedef struct Obj {
    char pad0[0x1B8];
    int x1B8;
    int x1BC;
    char pad1C0[0x3AC - 0x1C0];
    int x3AC;
    int x3B0;
    char pad3B4[0x42B - 0x3B4];
    unsigned char x42B;
    unsigned char x42C;
} Obj;

extern void func_0017EB00(Obj *obj, int id, int value);
extern int func_00220AE0(int axis, int value);

void func_0017E300(Obj *obj, int value)
{
    int v;

    func_0017EB00(obj, 9, value);
    func_0017EB00(obj, 10, value);
    obj->x42B = 0;
    obj->x42C = 0;
    if (obj->x3AC != 0 || obj->x3B0 != 0) {
        return;
    }
    if (obj->x1B8 == 0 && obj->x1BC == 0) {
        v = func_00220AE0(3, value);
        obj->x1B8 = v;
        obj->x1BC = 0;
        if (v < 0) {
            obj->x1BC = v;
            obj->x1B8 = 0;
        }
        if (obj->x1B8 != 0) {
            if (obj->x1B8 < 127) {
                obj->x1B8 = obj->x1B8 * obj->x1B8 / 127;
            } else {
                obj->x1B8 = 127;
            }
        }
        if (obj->x1BC != 0) {
            if (obj->x1BC < 127) {
                obj->x1BC = obj->x1BC * obj->x1BC / 127;
            } else {
                obj->x1BC = 127;
            }
        }
        obj->x42B = obj->x1B8 != 0;
        obj->x42C = obj->x1BC != 0;
    } else {
        if (obj->x1B8 != 0) {
            obj->x1B8 = 1;
        }
        if (obj->x1BC != 0) {
            obj->x1BC = 1;
        }
    }
}
