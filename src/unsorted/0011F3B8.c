/* UNIT 0x0011F3B8 0x0011F4C0 */
typedef struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x250 - 0xC];
    float x250;
    char pad254[0x26A - 0x254];
    short x26A;
} Obj;

extern void func_0011E568(Obj *, int);
extern int func_00222BD8(int, int);

void func_0011F3B8(Obj *obj) {
    int state;

    if (obj->x250 < 225.0f && obj->x26A >= 301) {
        func_0011E568(obj, 6);
        return;
    }
    state = obj->x8;
    if (state == 6) {
        return;
    }
    if (obj->x250 < 6400.0f) {
        func_0011E568(obj, 5);
    } else if (state == 5) {
        if (obj->x250 > 32400.0f && func_00222BD8(0, 100) == 42) {
            func_0011E568(obj, 2);
        }
    } else if (state < 2 || state > 4) {
        func_0011E568(obj, 4);
    }
}
