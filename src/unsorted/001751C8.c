/* UNIT 0x001751C8 0x001755F0 .rodata=0x28B8B0:0x28B940 */
typedef struct Effect {
    char pad0[0x4];
    char x4;
} Effect;

typedef struct Player {
    char pad0[0x117C];
    Effect *x117C;
    char pad1180[0x1380 - 0x1180];
} Player;

typedef struct Game {
    char pad0[0x144];
    int x144;
    char pad148[0x8];
    Player players[4];
    char pad4F50_[0x4F50 - 0x150 - 4 * 0x1380];
    float x4F50;
    float x4F54;
} Game;

typedef struct Settings {
    char pad0[0x288];
    int mode;
    char pad28C[0x2A0 - 0x28C];
    int track;
} Settings;

extern Settings D_004B25A0;

extern void func_00178170(Player *);
extern void func_0017BB18(Player *);
extern void func_00175998(Player *);
extern void func_00176278(Player *, int);
extern void func_00175960(Player *);
extern void func_00176E40(Player *, int);
extern void func_00175980(Player *);
extern void func_001755F0(Player *);

void func_001751C8(Game *obj)
{
    Player *player;
    Player *second;
    int i;

    player = obj->players;
    for (i = 3; i >= 0; i--) {
        func_00178170(player);
        if (player->x117C != 0) {
            player->x117C->x4 = 0;
        }
        func_0017BB18(player);
        func_00175998(player);
        player++;
    }
    func_00176278(&obj->players[0], 0);
    switch (D_004B25A0.track) {
    case 0:
        obj->x4F50 = 6211.0f;
        obj->x4F54 = -11463.0f;
        break;
    case 6:
        obj->x4F50 = -7932.0f;
        obj->x4F54 = -11225.0f;
        break;
    case 24:
        obj->x4F50 = 156.0f;
        obj->x4F54 = -7870.0f;
        break;
    case 11:
        obj->x4F50 = 315.0f;
        obj->x4F54 = -12635.0f;
        break;
    case 15:
        obj->x4F50 = 206.0f;
        obj->x4F54 = -2167.0f;
        break;
    case 16:
        obj->x4F50 = 472.0f;
        obj->x4F54 = -6560.0f;
        break;
    case 19:
        obj->x4F50 = 65.0f;
        obj->x4F54 = -11872.0f;
        break;
    case 25:
        obj->x4F50 = 99.0f;
        obj->x4F54 = -8038.0f;
        break;
    case 20:
        obj->x4F50 = 130.0f;
        obj->x4F54 = -8375.0f;
        break;
    case 17:
        obj->x4F50 = 35.0f;
        obj->x4F54 = -2210.0f;
        break;
    case 18:
        obj->x4F50 = 357.0f;
        obj->x4F54 = -3104.0f;
        break;
    case 21:
        obj->x4F50 = 5.0f;
        obj->x4F54 = -2714.0f;
        break;
    case 12:
        obj->x4F50 = 12182.0f;
        obj->x4F54 = 20.0f;
        break;
    case 5:
        obj->x4F50 = 5479.0f;
        obj->x4F54 = -11180.0f;
        break;
    case 14:
        obj->x4F50 = 39.0f;
        obj->x4F54 = -1877.0f;
        break;
    case 22:
        obj->x4F50 = 3.0f;
        obj->x4F54 = -7120.0f;
        break;
    case 2:
        obj->x4F50 = 11402.0f;
        obj->x4F54 = -5385.0f;
        break;
    case 8:
        obj->x4F50 = 48.0f;
        obj->x4F54 = -1794.0f;
        break;
    case 10:
        obj->x4F50 = 12678.0f;
        obj->x4F54 = 6935.0f;
        break;
    case 7:
        obj->x4F50 = -11763.0f;
        obj->x4F54 = -15000.0f;
        break;
    case 13:
        obj->x4F50 = -43.0f;
        obj->x4F54 = -5072.0f;
        break;
    case 3:
        obj->x4F50 = -434.0f;
        obj->x4F54 = -21234.0f;
        break;
    case 4:
        obj->x4F50 = 100.0f;
        obj->x4F54 = -6031.0f;
        break;
    case 9:
        obj->x4F50 = 206.0f;
        obj->x4F54 = -2240.0f;
        break;
    case 27:
        obj->x4F50 = 22.0f;
        obj->x4F54 = -965.0f;
        break;
    case 28:
        obj->x4F50 = 0.0f;
        obj->x4F54 = -800.0f;
        break;
    case 1:
    case 23:
    case 26:
    default:
        obj->x4F50 = 11370.0f;
        obj->x4F54 = -15048.0f;
        break;
    }
    if (D_004B25A0.mode == 2) {
        func_00175960(&obj->players[0]);
        func_00175960(&obj->players[1]);
        second = &obj->players[1];
    } else {
        second = &obj->players[1];
        if (D_004B25A0.mode == 1) {
            func_00176E40(second, 0);
            func_00175980(&obj->players[0]);
        }
    }
    obj->x144 = 0;
    func_001755F0(&obj->players[0]);
    func_001755F0(second);
}

/* In retail this string follows the jump table in the same .rodata, so it was
   defined in this file (also used by func_00175730 and func_0017A678). */
const char D_0028B928[] = "CONGRATULATIONS!";
