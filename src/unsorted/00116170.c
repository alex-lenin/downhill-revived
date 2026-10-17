/* UNIT 0x00116170 0x001161F8 */
typedef struct Game {
    char pad0[0x796C];
    int x796C;
} Game;

typedef struct Obj {
    char pad0[0x58];
    Game *x58;
    char pad5C[0x70];
    int xCC;
} Obj;

void func_00116170(Obj *obj)
{
    switch (obj->x58->x796C) {
    case 0x20:
    case 0x80:
    case 0xE0:
    case 0x120:
    case 0x140:
    case 0x1A0:
        obj->xCC |= 1;
        break;
    default:
        obj->xCC &= ~1;
        break;
    }
}
