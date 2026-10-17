/* UNIT 0x0011F760 0x0011F880 */
typedef struct Obj {
    char pad0[0x8];
    int state;
    char padC[0x250 - 0xC];
    float dist;
    char pad254[0x25C - 0x254];
    short x25C;
    char pad25E[0x264 - 0x25E];
    short x264;
    short x266;
} Obj;

extern void func_0011E568(Obj *obj, int state);
extern short func_00222BD8(int min, int max);

void func_0011F760(Obj *obj)
{
    if (obj->dist < 10000.0f) {
        func_0011E568(obj, 5);
        return;
    }
    if (obj->state == 5) {
        if (obj->dist > 22500.0f && func_00222BD8(0, 100) == 42) {
            func_0011E568(obj, 2);
        }
        return;
    }
    if ((unsigned int)(obj->state - 2) < 3) {
        if (obj->state == 4) {
            if (obj->x25C > obj->x264) {
                func_0011E568(obj, 3);
                obj->x266 = func_00222BD8(60, 300);
            }
        } else if (obj->state == 3 && obj->x25C > obj->x266) {
            goto set4;
        }
    } else {
    set4:
        func_0011E568(obj, 4);
        obj->x264 = func_00222BD8(120, 240);
    }
}
