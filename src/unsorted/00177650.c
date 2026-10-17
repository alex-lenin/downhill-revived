/* UNIT 0x00177650 0x00177768 */
typedef struct Rider {
    char pad0[0x798C];
    int x798C;
} Rider;

typedef struct Hud {
    char pad0[0x1164];
    Rider *x1164;
    char pad1168[0x1280 - 0x1168];
    int x1280;
} Hud;

extern int D_004B26AC[];
extern char D_0029C988[];
extern int func_00254D68(char *, char *, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0017B520(Hud *, char *, float, float);

void func_00177650(Hud *hud) {
    char text[16];

    if (D_004B26AC[0] == 0x11 || D_004B26AC[0] == 0xF) return;
    func_00254D68(text, D_0029C988, hud->x1164->x798C);
    if (hud->x1164->x798C == 1) {
        func_0020F588(hud->x1280, 0, 0x33, 0x4D, 0xCD, 0xAA);
    } else if (hud->x1164->x798C == 2) {
        func_0020F588(hud->x1280, 0, 0xD4, 0x3E, 0x3E, 0xAA);
    } else if (hud->x1164->x798C == 3) {
        func_0020F588(hud->x1280, 0, 0xFA, 0xFF, 0, 0xAA);
    } else {
        func_0020F588(hud->x1280, 0, 0, 0xB3, 0x33, 0xAA);
    }
    func_0020FB80(0, 0x226, 0x15E, -0x78, 0);
    func_0017B520(hud, text, 0.23f, 1.6f);
}
