/* UNIT 0x0017BEF0 0x0017C008 */
typedef struct Icon {
    int sprite;
    char visible;
} Icon;

typedef struct Gauge {
    char pad0[0x20];
    int items[3];
} Gauge;

typedef struct Hud {
    char pad0[0xF64];
    Gauge *xF64;
    char padF68[0x116C - 0xF68];
    Icon *x116C;
    char pad1170[0x127C - 0x1170];
    int x127C;
    int x1280;
} Hud;

extern int D_004B2828[];
extern char D_0029C988[];
extern int func_00254D68(char *, char *, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0017B520(Hud *, char *, float, float);

void func_0017BEF0(Hud *hud, int time) {
    char text[16];
    int seconds;

    if (D_004B2828[0] >= 3) {
        int count = time / 60;
        if (count > 0 && count < 4) {
            func_00254D68(text, D_0029C988, count);
            func_0020F588(hud->x1280, 0, 0x14, 0x14, 0x14, 0xBE);
            func_0020FB80(0, 0x320, 0x1F4, -0x78, 0);
            func_0017B520(hud, text, 1.0f, 0.25f);
        }
    } else if (hud->x127C && hud->x116C) {
        seconds = time / 60;
        if (seconds > 0) {
            if (seconds < 4) {
                hud->x116C->sprite = hud->xF64->items[seconds - 1];
                hud->x116C->visible = 1;
            } else {
                hud->x116C->visible = 0;
            }
        } else {
            hud->x116C->visible = 0;
        }
    }
}
