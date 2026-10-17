/* UNIT 0x00116FA0 0x00117090 */
struct State { char pad0[0x796C]; int x796C; };
struct Obj { char pad0[0x58]; struct State *x58; };
int func_00116FA0(struct Obj *obj) {
    switch (obj->x58->x796C) {
    case 0xA0: case 0x160: return 75;
    case 0x60: case 0x100: return 15;
    case 0x20: case 0x120: return 10;
    case 0xE0: case 0x140: return 7;
    case 0xC0: return 5;
    case 0x1A0: return 3;
    case 0x1C0:
    case 0x200: default: return 0;
    }
}
