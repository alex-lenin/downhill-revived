/* UNIT 0x001E7DA0 0x001E7E80 */
typedef struct Sub {
    char pad0[0x10E4];
    unsigned char x10E4;
} Sub;

typedef struct Game {
    char pad0[0x40C0];
    char x40C0[0x6610 - 0x40C0];
    Sub x6610;
    char pad[0x78D4 - 0x6610 - sizeof(Sub)];
    int x78D4;
} Game;

typedef struct Obj {
    Game *game;
    char pad4[0x2C - 4];
    int x2C;
    char pad30[0x1EC - 0x30];
    int x1EC;
    char pad1F0[4];
    unsigned char x1F4;
} Obj;

extern int func_001E5900(Sub *);
extern int func_001E68C0(Sub *);
extern int func_001E6E28(Sub *);
extern int func_00138560(void *);
extern void func_001E5990(Sub *, int);

int func_001E7DA0(Obj *obj)
{
    int ok = 1;
    Sub *sub = &obj->game->x6610;
    int result;

    if (obj->game->x78D4) {
        if (!obj->x1EC) {
            return 0;
        }
        if (!func_001E5900(sub)) {
            return 0;
        }
        obj->x2C = func_001E68C0(sub);
        return 1;
    }
    if (!obj->x1F4) {
        return 0;
    }
    if (!func_001E5900(sub)) {
        return 0;
    }
    sub->x10E4 = 1;
    if (!func_001E6E28(sub) || !(unsigned char)func_00138560(obj->game->x40C0)) {
        func_001E5990(sub, 0);
        return 0;
    }
    result = func_001E68C0(sub);
    obj->x2C = result;
    if (!result) {
        ok = 0;
    }
    return ok;
}
