/* UNIT 0x00117470 0x00117598 */
typedef struct State {
    char pad0[0x796C];
    int x796C;
} State;

typedef struct Obj {
    char pad0[0x58];
    State *x58;
} Obj;

int func_00117470(Obj *obj)
{
    switch (obj->x58->x796C) {
    case 0xA0:
    case 0x160:
        return 50;
    case 0x40:
    case 0xC0:
        return 0;
    case 0x20:
    case 0x60:
    case 0x80:
    case 0xE0:
    case 0x100:
    case 0x120:
    case 0x140:
    case 0x180:
    case 0x1A0:
        return 12;
    case 0x1C0:
    case 0x1E0:
    default:
        return 4;
    }
}
