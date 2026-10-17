/* UNIT 0x00175730 0x00175960 */
typedef struct Rider {
    char pad0[0x79AC];
    int x79AC;
    char pad79B0[0x7BD0 - 0x79B0];
} Rider;

typedef struct Hud {
    char pad0[0x1164];
    Rider *x1164;
    char pad1168[0x1280 - 0x1168];
    int x1280;
    char pad1284[0x1364 - 0x1284];
    int x1364;
} Hud;

typedef struct Config {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x364 - 0x28C];
    unsigned int x364;
} Config;

typedef struct World {
    Rider racers[10];
    char pad4D620[0x501A8 - 10 * 0x7BD0];
    int x501A8;
} World;

extern Config D_004B25A0;
extern World D_002AF2C0;
extern char D_0028B928[];
extern char D_0028B940[];

extern void func_0023D650(int handle, int *w, int *h);
extern int func_0023DED0(void);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020F9F0(int a, int handle, int x, int y, char *text);
extern float func_001301B8(Rider *);

void func_00175730(Hud *hud, int arg)
{
    int w;
    int h;
    int x;
    int y;
    Config *cfg;
    int ok;
    int own;
    int first;
    int i;
    int n;
    Rider *racer;

    if (hud->x1364 <= 0) {
        return;
    }
    y = 0x48;
    func_0023D650(hud->x1280, &w, &h);
    x = w / 2;
    if (func_0023DED0() == 2) {
        y = 0x32;
    }
    if (D_004B25A0.x288 >= 2) {
        func_0020FB80(0, 0x140, 0xE0, -0x7B, -0x20);
    } else {
        func_0020FB80(0, 0x180, 0x120, -0x7B, -0x20);
    }
    func_0020F588(hud->x1280, 0, 0x63, 0x68, 0xCE, hud->x1364);
    cfg = &D_004B25A0;
    ok = 0;
    if (cfg->x10C == 15) {
        ok = (cfg->x364 == 0);
    }
    if (ok) {
        own = (int)func_001301B8(hud->x1164);
        first = 1;
        for (i = 0; i < D_002AF2C0.x501A8; i++) {
            racer = &D_002AF2C0.racers[i];
            if (racer != hud->x1164 && (int)func_001301B8(racer) > own) {
                first = 0;
            }
        }
        if (first == 0) {
            func_0020F9F0(0, hud->x1280, x, y, D_0028B940);
        } else {
            func_0020F9F0(0, hud->x1280, x, y, D_0028B928);
        }
    } else {
        n = 1;
        if (hud->x1164->x79AC <= n) {
            func_0020F9F0(0, hud->x1280, x, y, D_0028B928);
        } else {
            func_0020F9F0(0, hud->x1280, x, y, D_0028B940);
        }
    }
    hud->x1364 += arg;
    if (hud->x1364 >= 0xAA) {
        hud->x1364 = 0xAA;
    }
}
