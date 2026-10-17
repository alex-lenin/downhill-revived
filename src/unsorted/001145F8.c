/* UNIT 0x001145F8 0x001146F0 */
typedef struct Game { char pad0[0x796C]; int x796C; } Game;
typedef struct Obj {
    char pad0[0x58];
    Game *x58;
    char pad5C[0x4];
    float x60;
} Obj;

/* x60 = an angle in radians (5, 15, 20 or 30 degrees) chosen from x58->x796C.
   The cases that share the default label are needed: gcc drops their compares,
   but they shape the decision tree. */
void func_001145F8(Obj *obj)
{
    obj->x60 = 0.0f;
    switch (obj->x58->x796C) {
    case 0xA0:
    case 0x160:
        obj->x60 = 0.08726646f;
        break;
    case 0x40:
    case 0xC0:
        obj->x60 = 0.2617994f;
        break;
    case 0xE0:
    case 0x100:
    case 0x140:
        obj->x60 = 0.34906584f;
        break;
    case 0x60:
    case 0x80:
    case 0x120:
    case 0x180:
    case 0x1A0:
    case 0x1C0:
    case 0x1E0:
    default:
        obj->x60 = 0.5235988f;
        break;
    }
}
