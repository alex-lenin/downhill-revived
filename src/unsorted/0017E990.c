/* UNIT 0x0017E990 0x0017EAE0 */
typedef struct Obj {
    char pad0[0x154];
    void *x154;
    char pad158[0x174 - 0x158];
    void *x174;
    char pad178[0x1E8 - 0x178];
    int x1E8;
    char pad1EC[0x1F0 - 0x1EC];
    int x1F0;
    char pad1F4[0x405 - 0x1F4];
    unsigned char x405;
    char pad406;
    unsigned char x407;
    char pad408[0x437 - 0x408];
    unsigned char x437;
    char pad438;
    unsigned char x439;
} Obj;

extern void func_0017EB00(Obj *obj, int type, int arg);
extern void func_0017EDE8(Obj *obj, int type, int arg);
extern int func_00220618(void *p, int value);
extern int func_00220AE0(int, int);

void func_0017E990(Obj *obj, int value)
{
    int axis;

    if (!obj->x437) {
        func_0017EB00(obj, 0x15, value);
    }
    if (!obj->x439) {
        func_0017EB00(obj, 0x17, value);
    }
    if (!obj->x437 && !obj->x439) {
        if (func_00220618(obj->x154, value)) {
            return;
        }
        if (func_00220618(obj->x174, value)) {
            return;
        }
    }
    obj->x437 = 0;
    obj->x439 = 0;
    axis = func_00220AE0(3, value);
    obj->x1E8 = axis;
    obj->x1F0 = 0;
    if (axis < 0) {
        obj->x1E8 = 0;
        obj->x1F0 = axis;
    }
    if (obj->x1E8) {
        if (obj->x1E8 < 127) {
            obj->x1E8 = 0;
        } else {
            obj->x1E8 = 127;
        }
    }
    if (obj->x1F0) {
        if (obj->x1F0 > -127) {
            obj->x1F0 = 0;
        } else {
            obj->x1F0 = 127;
        }
    }
    obj->x437 = obj->x1E8 != 0;
    obj->x439 = obj->x1F0 != 0;
    if (obj->x405) {
        func_0017EDE8(obj, 0x15, value);
    }
    if (obj->x407) {
        func_0017EDE8(obj, 0x17, value);
    }
}
