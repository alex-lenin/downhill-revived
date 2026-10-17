/* UNIT 0x0015D578 0x0015D958 */
typedef struct Record {
    int value;
    short x4, x6;
    unsigned char x8, x9, xA, xB;
    char name[12];
} Record;

typedef struct Slot {
    Record tricks;
    Record total;
    char pad30[0x48 - 0x30];
    long x48;
    long x50;
    int x58;
    Record first;
    Record second;
    Record third;
    char padA4[0xD8 - 0xA4];
} Slot;

typedef struct Stats {
    char pad0[0x7668];
    Slot slots[1];
} Stats;

typedef struct Game {
    char pad0[0x2A0];
    int x2A0;
} Game;

typedef struct MenuState {
    char pad0[0x138];
    int x138;
} MenuState;

typedef struct UiObj {
    char pad0[0xE4];
    void *xE4;
} UiObj;

extern int D_003F52F0[];
extern UiObj *D_0029DE68;
extern char D_004225A0[];
extern Game D_004B25A0;
extern MenuState D_0043F870;

extern char D_0028A3C0[];
extern char D_0028A3D8[];
extern char D_0028A400[];
extern char D_0029C710[];
extern char D_0029C718[];
extern char D_0029C720[];
extern char D_0029C728[];
extern char D_0029C730[];
extern char D_0029C738[];
extern char D_0029C740[];
extern char D_0029C748[];
extern char D_0029C750[];
extern char D_0029C758[];
extern char D_0029C760[];
extern char D_0029C768[];

extern void func_0023D650(int, int *, int *);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F648(int, int, int);
extern void func_0018D930(void *, int, int, void *);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9A8(int, int, int, int, const char *);
extern int func_00254D68(char *, const char *, ...);
extern long func_001FBDD8(long, long);
extern void func_0015D958(int, int, Record *, char *, int, int, int);
extern void func_0018DC20(void *);

void func_0015D578(void *obj)
{
    char text[32];
    int width;
    int height;
    Game *game;
    Stats *stats;
    int font;
    int trick_value;
    int stride;

    font = D_003F52F0[0];
    func_0023D650(font, &width, &height);
    width /= 2;

    func_0020F518(0, 200, 200, 0, 128);
    func_0020F648(0, 36, 24);
    func_0018D930(obj, width, 6, D_0029DE68->xE4);
    func_0020FB80(0, 240, 192, -96, -32);
    func_0020F518(0, 110, 110, 200, 150);

    game = &D_004B25A0;
    stats = (Stats *)D_004225A0;
    stride = 0xD8;
    func_00254D68(text, D_0028A3C0,
                  *(int *)((char *)stats + game->x2A0 * stride + 0x76C0));
    func_0020F9A8(0, font, 140, 38, text);

    {
        int value;
        int seconds;
        int minutes;
        int hours;
        int total_base;
        int count_base;

        /* each base is built in place (mult, then add), so cse cannot
           share the product between the two: retail has two madds */
        total_base = game->x2A0 * stride;
        total_base += (int)stats;
        count_base = game->x2A0 * stride;
        count_base += (int)stats;
        /* func_001FBDD8 is libgcc __divdi3: average race time */
        value = func_001FBDD8(*(long *)(total_base + 0x76B0),
                              *(int *)(count_base + 0x76C0));
        seconds = value % 60;
        value /= 60;
        minutes = value % 60;
        value /= 60;
        hours = value % 60;
        func_00254D68(text, D_0028A3D8, hours, minutes, seconds);
    }

    {
        int total_base;
        int count_base;

        total_base = game->x2A0 * stride;
        total_base += (int)stats;
        count_base = game->x2A0 * stride;
        count_base += (int)stats;
        trick_value = func_001FBDD8(*(long *)(total_base + 0x76B8),
                                    *(int *)(count_base + 0x76C0));
    }

    func_0020F9A8(0, font, 80, 48, text);
    func_00254D68(text, D_0028A400, trick_value);
    func_0020F9A8(0, font, 68, 58, text);

    func_0020F9A8(0, font, 20, 76, D_0029C710);
    func_0020F9A8(0, font, 84, 76, D_0029C718);
    func_0020F9A8(0, font, 156, 76, D_0029C720);
    func_0020F9A8(0, font, 218, 76, D_0029C728);
    func_0020F9A8(0, font, 294, 76, D_0029C730);
    func_0020F9A8(0, font, 360, 76, D_0029C738);
    func_0020F9A8(0, font, 420, 76, D_0029C740);

    func_0015D958(88, 20, (Record *)((char *)stats + game->x2A0 * stride + 0x76C4),
                  D_0029C748, font, 1, D_0043F870.x138 & 1);
    func_0015D958(100, 20, (Record *)((char *)stats + game->x2A0 * stride + 0x76DC),
                  D_0029C750, font, 1, D_0043F870.x138 & 2);
    func_0015D958(112, 20, (Record *)((char *)stats + game->x2A0 * stride + 0x76F4),
                  D_0029C758, font, 1, D_0043F870.x138 & 4);
    func_0015D958(124, 20, (Record *)((char *)stats + game->x2A0 * stride + 0x7668),
                  D_0029C760, font, 0, D_0043F870.x138 & 8);
    func_0015D958(136, 20, (Record *)((char *)stats + game->x2A0 * stride + 0x7680),
                  D_0029C768, font, 0, D_0043F870.x138 & 16);
    func_0018DC20(obj);
}
