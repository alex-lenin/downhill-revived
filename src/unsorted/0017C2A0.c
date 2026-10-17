/* UNIT 0x0017C2A0 0x0017C638 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Track {
    char pad0[0x18];
    float x18;
} Track;

typedef struct Rider {
    char pad0[0x4180];
    Vec velocity;
    char pad4190[0x7720 - 0x4190];
    char x7720[0x792C - 0x7720];
    Track *x792C;
    char pad7930[0x796C - 0x7930];
    int x796C;
    char pad7970[0x7988 - 0x7970];
    unsigned int x7988;
    int x798C;
    char pad7990[0x79B0 - 0x7990];
    int x79B0[(0x7A5C - 0x79B0) / 4];
    int x7A5C;
    char pad7A60[0x7BD0 - 0x7A60];
} Rider;

typedef struct Global {
    Rider riders[10];
    char pad[0x501A8 - 10 * 0x7BD0];
    int count;
} Global;

typedef struct Panel {
    char pad0[0x20];
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
} Panel;

typedef struct Panels {
    char pad0[0x20];
    Panel *x20[4];
} Panels;

typedef struct Quad {
    int x[4];
} Quad;

typedef struct Hud {
    char pad0[0xDE4];
    Quad xDE4[(0xF84 - 0xDE4) / 16];
    Panels *xF84;
    int xF88;
    char padF8C[0x1164 - 0xF8C];
    Rider *x1164;
    char pad1168[0x1244 - 0x1168];
    int x1244[(0x131C - 0x1244) / 4];
    int x131C;
} Hud;

extern int D_004B2828[];
extern Global D_002AF2C0;
extern Rider *D_0043EEC0[];
extern char D_004C8508[];
extern char D_0029CA90[];
extern char D_003B2270[];

extern void func_001C7940(char *p);
extern int func_001FA258(char *table, int lap, int course);
extern int func_00254D68(char *buf, char *fmt, int value);
extern void func_0017B338(char *obj, int id, int priority, int flag, int offset, char *text);
extern int func_00215B58(int id);
extern int func_0017C830(Hud *hud, int p);
extern void func_0017C8A8(Hud *hud, Quad *src, int dst);
extern void func_0017C7A0(Hud *hud, int arg, int number);
extern void func_0017C6B0(Hud *hud, int arg1, int arg2, int value);

static inline float Length(Vec *v) {
    float r;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                      : "=&f"(r) : "f"(v->x), "f"(v->y), "f"(v->z));
    return r;
}

/* banners are 0x1380 bytes apart, one per player */
static inline void ShowBanner(char *banners, unsigned int player, int id, char *text) {
    if (player < 4) func_0017B338(banners + (player * 0x1380 + 0x150), id, 1, 0, 0x14, text);
}

void func_0017C2A0(Hud *hud) {
    char text[16];
    int count;
    int base;
    int i;
    int lap;
    int idx;
    int time;
    int frames, secs, mins;
    int line;
    int value;
    float dist;
    float start;
    Rider *rider;
    Rider *riders;
    Panel *panel;
    char *str;

    if (D_004B2828[0] < 3) {
        count = D_002AF2C0.count;
        rider = D_0043EEC0[0];
        if (rider == 0) return;
        start = rider->x792C->x18;
        base = rider->x79B0[hud->x131C];

        for (i = 0; i < count; i++) {
            riders = D_002AF2C0.riders;
            rider = &riders[i];
            if (rider->x7A5C == 0) continue;
            lap = rider->x798C;
            if (lap <= 0) continue;
            if (lap >= 5) continue;
            if (lap < 4 && rider->x7A5C == 1 && rider == hud->x1164) {
                func_001C7940(rider->x7720);
                str = (char *)func_001FA258(D_004C8508, lap, (rider->x796C >> 5) - 1);
                if (str != 0) {
                    func_00254D68(text, D_0029CA90, (int)str);
                    ShowBanner(D_003B2270, rider->x7988, lap + 0x1F, text);
                }
            }
            idx = lap - 1;
            if (hud->xF88 != 0) {
                line = rider->x796C / 32 - 1;
                panel = hud->xF84->x20[idx];
                value = func_0017C830(hud, func_00215B58(panel->x20));
                func_0017C8A8(hud, &hud->xDE4[line], value);
            }
            if (rider->x7A5C == 1) hud->x1244[idx * 2] = 1;
            else hud->x1244[idx * 2] = 0;
            if (lap == 1) time = base;
            else {
                dist = rider->x792C->x18 - start;
                time = base + (int)(dist / (Length(&hud->x1164->velocity) * 0.016666668f));
            }
            if (time != 0) {
                if (lap >= 2) {
                    secs = time - base;
                    frames = secs % 60;
                    secs = secs / 60;
                } else {
                    frames = time % 60;
                    secs = time / 60;
                }
                mins = secs / 60 % 60;
                secs = secs % 60;
            } else {
                mins = -1;
                secs = -1;
                frames = -1;
            }
            panel = hud->xF84->x20[idx];
            if (lap == 1) {
                func_0017C7A0(hud, panel->x24, mins);
                func_0017C6B0(hud, panel->x28, panel->x2C, secs);
                func_0017C6B0(hud, panel->x30, panel->x34, frames);
            } else {
                func_0017C6B0(hud, panel->x24, panel->x28, secs);
                func_0017C6B0(hud, panel->x2C, panel->x30, frames);
            }
        }
    }
}
