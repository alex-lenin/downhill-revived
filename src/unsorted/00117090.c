/* UNIT 0x00117090 0x00117218 */
struct State { char pad[0x796c]; int x796c; };
struct Flags { char pad[0x440]; unsigned long x440; };
struct Obj { char pad[0x58]; struct State *x58; struct Flags *x5c; };
extern int func_00222BD8(int, int);
int func_00117090(struct Obj *obj) {
    int result;
    int flag = 0;
    switch (obj->x58->x796c) {
    case 0xa0: case 0x160: result = 30; flag = 1; break;
    case 0x60: result = 10; flag = func_00222BD8(0, 1) != 0; break;
    case 0x20: case 0x120: result = 8; flag = func_00222BD8(0, 3) == 1; break;
    case 0xe0: case 0x100: case 0x140: result = 5; flag = func_00222BD8(0, 5) == 1; break;
    case 0x80: result = 3; break;
    case 0x180: case 0x1a0: result = 1; break;
    case 0x1c0: case 0x1e0: default: result = 0; break;
    }
    if (obj->x5c) obj->x5c->x440 = (obj->x5c->x440 & ~(1UL << 33)) | ((unsigned long)flag << 33);
    return result;
}
