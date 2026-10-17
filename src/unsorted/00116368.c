/* UNIT 0x00116368 0x00116828 */
typedef struct State { char pad[0x796C]; int x796C; } State;
typedef struct Obj {
    char pad[0x58];
    State *x58;
    char pad5C[0xB8 - 0x5C];
    int xB8;
    int xBC;
    int xC0;
} Obj;
extern struct Globals { char pad[0x5BB4]; int x5BB4; char pad5BB8[0x24]; int x5BDC; } D_00398C40;
extern int func_00222BD8(int min, int max);

static inline int GetDifficulty(void) {
    if (D_00398C40.x5BB4 == 0) return D_00398C40.x5BDC / 2;
    else if (D_00398C40.x5BB4 == 2) return D_00398C40.x5BDC / 2 + 50;
    else return D_00398C40.x5BDC / 2 + 25;
}

void func_00116368(Obj *obj) {
    int limit;

    obj->xC0 = 0;
    obj->xB8 = 0;
    obj->xBC = 0;
    switch (obj->x58->x796C) {
    case 0x20:
    case 0x120:
        obj->xB8 = 180; obj->xBC = 95; obj->xC0 = 10;
        break;
    case 0x40:
        obj->xB8 = 600; obj->xBC = 50; obj->xC0 = 50;
        break;
    case 0x60:
        obj->xB8 = 1200; obj->xBC = 5; obj->xC0 = 95;
        break;
    case 0xC0:
        obj->xB8 = 1800; obj->xBC = 80; obj->xC0 = 95;
        break;
    case 0x80:
    case 0xE0:
    case 0x100:
    case 0x140:
    case 0x180:
    case 0x1A0:
        obj->xB8 = 2700; obj->xBC = 75; obj->xC0 = 5;
        break;
    case 0x1C0:
        goto fallback;
    case 0xA0:
    case 0x160:
    case 0x1E0:
    default:
    fallback:
        obj->xB8 = 18000; obj->xBC = 95;
        break;
    }
    obj->xB8 += func_00222BD8(-obj->xB8 / 4, obj->xB8 / 4);
    if (GetDifficulty() < 16) {
        limit = 100;
        obj->xB8 = obj->xB8 * 3;
    } else if (GetDifficulty() < 33) {
        limit = 100;
        obj->xB8 = obj->xB8 * 2;
    } else if (GetDifficulty() < 50) {
        limit = 80;
    } else if (GetDifficulty() < 66) {
        limit = 75;
        obj->xB8 = obj->xB8 * 2 / 3;
    } else if (GetDifficulty() < 83) {
        limit = 65;
        obj->xB8 = obj->xB8 / 3;
    } else if (GetDifficulty() < 100) {
        limit = 60;
        obj->xB8 = obj->xB8 / 4;
    } else {
        limit = 50;
        obj->xB8 = obj->xB8 / 5;
    }
    if (obj->xBC > limit) obj->xBC = limit;
    else if (obj->xBC < 100 - limit) obj->xBC = 100 - limit;
    if (obj->xC0 > limit) obj->xC0 = limit;
    else if (obj->xC0 < 100 - limit) obj->xC0 = 100 - limit;
}
