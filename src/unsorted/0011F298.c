/* UNIT 0x0011F298 0x0011F3B8 */
typedef struct Obj {
    char pad0[0x8];
    int state;
    char padC[0x244];
    float x250;
    char pad254[0x8];
    short x25C;
    char pad25E[0x6];
    short x264;
    short x266;
} Obj;

extern void func_0011E568(Obj *, int);
extern short func_00222BD8(int, int);

void func_0011F298(Obj *obj)
{
    if (obj->x250 < 1225.0f) {
        func_0011E568(obj, 5);
        return;
    }
    if (obj->state == 5) {
        if (obj->x250 > 5625.0f && func_00222BD8(0, 100) == 42) {
            func_0011E568(obj, 2);
        }
    } else if (obj->state >= 2 && obj->state <= 4) {
        if (obj->state == 4) {
            if (obj->x25C > obj->x264) {
                func_0011E568(obj, 3);
                obj->x266 = func_00222BD8(60, 300);
            }
        } else if (obj->state == 3 && obj->x25C > obj->x266) {
            goto idle;
        }
    } else {
    idle:
        func_0011E568(obj, 4);
        obj->x264 = func_00222BD8(300, 900);
    }
}
