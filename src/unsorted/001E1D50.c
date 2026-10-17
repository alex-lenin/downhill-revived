/* UNIT 0x001E1D50 0x001E1E98 */
typedef struct Rider {
    char pad0[0x7908];
    short x7908;
    char pad790A[0x796C - 0x790A];
    int x796C;
    char pad7970[0x7998 - 0x7970];
    int x7998;
} Rider;

typedef struct Obj {
    char pad0[0x30];
    int x30;
    int x34;
    int x38;
    int x3C;
} Obj;

typedef struct World {
    char pad0[0x50130];
    Rider *x50130;
} World;

typedef struct Slot {
    char pad0[0xD8];
} Slot;

typedef struct Players {
    char pad0[0x7668];
    Slot slots[1];
} Players;

typedef struct Game {
    char pad0[0x2A0];
    int x2A0;
} Game;

typedef struct Timer {
    char pad0[0x1C];
    int x1C;
} Timer;

extern World D_002AF2C0;
extern char D_0032FC60[];
extern char D_00398C40[];
extern Players D_004225A0;
extern Timer D_0043EEC0;
extern Game D_004B25A0;

extern void func_00139AF0(char *);
extern void func_00195570(Slot *, int, int, int, int, int);
extern void func_001C1548(char *, Rider *);

void func_001E1D50(Obj *obj) {
    Game *game = &D_004B25A0;
    Rider *rider = D_002AF2C0.x50130;
    Timer *timer = &D_0043EEC0;
    int offset2 = 0x5B74;
    int offset = game->x2A0 * 0xD8 + 0x7668;
    Slot *slot = (Slot *)((char *)&D_004225A0 + offset);

    obj->x3C = 0;
    if (obj->x30 == 0 || timer->x1C < obj->x30) {
        func_00139AF0(D_0032FC60);
        if (rider != 0) {
            func_00195570(slot, game->x2A0, timer->x1C, rider->x796C, rider->x7908, rider->x7998);
            if (obj->x30 != 0x0FFFFFFF) {
                obj->x3C = 1;
            }
            if (obj->x3C != 0) {
                func_001C1548(D_00398C40 + offset2, rider);
            }
        }
    } else if (obj->x38 == 1 && timer->x1C < obj->x34) {
        func_00139AF0(D_0032FC60);
        if (rider != 0) {
            func_00195570(slot, game->x2A0, timer->x1C, rider->x796C, rider->x7908, rider->x7998);
        }
    }
}
