/* UNIT 0x001FADD0 0x001FAE90 */
struct Player {
    char pad0[0x11D0];
    int x11D0;
    char pad11D4[0x1298 - 0x11D4];
    int x1298;
    char pad129C[0x13E7 - 0x129C];
    unsigned char x13E7;
    char pad13E8[0x40C0 - 0x13E8];
    char x40C0[0x4580 - 0x40C0];
    char x4580[0x48B0 - 0x4580];
    int x48B0;
    char pad48B4[0x4D50 - 0x48B4];
    char x4D50;
};

struct Obj {
    struct Player *x0;
    char pad4[0x4];
    int x8;
    char xC;
    char padD[0xF];
    int x1C;
    int x20;
    int x24;
    int x28;
};

extern void func_001ACFA0(void *, int);
extern void func_001AD580(void *, int, float, float);
extern void func_00136030(void *, int);

void func_001FADD0(struct Obj *obj)
{
    struct Player *player;
    int active;

    player = obj->x0;
    if (player->x13E7 != 0) {
        active = player->x1298;
    } else {
        active = player->x11D0;
    }
    if (active != 0) {
        obj->x28 = 1;
        func_001ACFA0(player->x4580, 1);
        func_001AD580(player->x4580, 1, 0.0f, 1.0f);
    } else {
        obj->x28 = 0;
        func_001ACFA0(player->x4580, 0);
    }
    player->x4D50 = 0;
    player->x48B0 = 1;
    func_00136030(player->x40C0, 1);
    obj->xC = 0;
    obj->x20 = 0;
    obj->x1C = 0;
    obj->x8 = 0;
    obj->x24 = 0;
}
