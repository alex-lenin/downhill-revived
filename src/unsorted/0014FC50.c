/* UNIT 0x0014FC50 0x00150148 .rodata=0x289FB0:0x28A010 .sdata=0x29C5D8:0x29C5F8 */
typedef struct Upgrades {
    char pad0[0x30];
    int frame;
    int fork;
    int wheel;
} Upgrades;

typedef struct Profile {
    char pad0[0x78D8];
    Upgrades upgrades;
} Profile;

typedef struct Bike {
    char pad0[0x796C];
    int model;
} Bike;

extern int D_004B26AC[];
typedef struct Game {
    char pad0[0x50130];
    Profile *profile;
} Game;

extern Game D_002AF2C0;
typedef struct Fonts {
    char pad0[0x20];
    int x20;
} Fonts;

extern Fonts D_003F52D0;
extern char D_004C8508[];
extern int D_00275EA0[];
extern char D_00276DF8[][30];
extern int D_00275E38[];
extern char D_002764A8[][40];
extern int D_00275E08[];
extern char D_002761E8[][32];

extern void func_0023D650(int, int *, int *);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00254D68(char *, char *, ...);
extern unsigned long func_001F9FA0(char *);
extern int func_001FA038(char *);
extern int func_001FA0D0(char *);

static inline int Get_frame(Upgrades *up) { return up->frame; }
static inline int Get_fork(Upgrades *up) { return up->fork; }
static inline int Get_wheel(Upgrades *up) { return up->wheel; }

void func_0014FC50(void)
{
    char buf[48];
    Game *game = &D_002AF2C0;
    int mode = D_004B26AC[0];
    int width, height;
    int font = D_003F52D0.x20;
    Profile *profile = game->profile;
    int center;
    int model;
    int pro;
    int y;
    int i;
    unsigned long mask;
    unsigned long bit;
    unsigned int mask2;
    unsigned int bit2;

    if (mode >= 6) {
        if (mode < 10) {
            return;
        }
    }
    func_0023D650(font, &width, &height);
    center = width / 2;
    pro = 0;
    model = ((Bike *)game->profile)->model;
    if (model == 0xE0 || model == 0x100 || model == 0x140 || model == 0x180 || model == 0x1A0) {
        pro = 1;
    }
    if (pro) {
        func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
        func_0020FB80(0, 0x160, 0x100, -0x72, -0x20);
        func_0020F9F0(0, font, center, 6, "NO UPGRADES AVAILABLE");
        func_0020F9F0(0, font, center, 0x16, "FOR PRO RIDER BIKE");
        return;
    }
    func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
    func_0020FB80(0, 0x160, 0x100, -0x72, -0x20);
    func_0020F9F0(0, font, center, 6, "UPGRADES AVAILABLE IN BIKE SHOP");
    y = 0x1D;
    func_0020FB80(0, 0xF0, 0xC0, -0x5B, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0xA, 0x16, "FRAMES");
    func_0020F9A8(0, font, 0x190, 0x16, "COST");
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    mask = func_001F9FA0(D_004C8508);
    for (i = 0; i < 0x26; i++) {
        if ((mask & (bit = 1UL << i)) && Get_frame(&profile->upgrades) != i) {
            func_0020F9A8(0, font, 0xA, y, D_00276DF8[i]);
            func_00254D68(buf, "$%d", D_00275EA0[i]);
            func_0020F9A8(0, font, 0x190, y, buf);
            y += 9;
        }
    }
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0xA, y + 3, "FORKS");
    func_0020F9A8(0, font, 0x190, y + 3, "COST");
    y += 0xB;
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    mask2 = func_001FA038(D_004C8508);
    for (i = 0; i < 0x19; i++) {
        if ((mask2 & (bit2 = 1U << i)) && Get_fork(&profile->upgrades) != i) {
            func_0020F9A8(0, font, 0xA, y, D_002764A8[i]);
            func_00254D68(buf, "$%d", D_00275E38[i]);
            func_0020F9A8(0, font, 0x190, y, buf);
            y += 9;
        }
    }
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0xA, y + 3, "WHEELS/TIRES");
    func_0020F9A8(0, font, 0x190, y + 3, "COST");
    y += 0xB;
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    mask2 = func_001FA0D0(D_004C8508);
    for (i = 0; i < 0xB; i++) {
        if ((mask2 & (bit2 = 1U << i)) && Get_wheel(&profile->upgrades) != i) {
            func_0020F9A8(0, font, 0xA, y, D_002761E8[i]);
            func_00254D68(buf, "$%d", D_00275E08[i]);
            func_0020F9A8(0, font, 0x190, y, buf);
            y += 9;
        }
    }
}
