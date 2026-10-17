/* UNIT 0x00179FA8 0x0017A0E0 .rodata=0x28BC58:0x28BC68 .sdata=0x29CA88:0x29CA90 */
typedef struct Hud {
    char pad0[0x1280];
    int player;
    char pad1284[0xE0];
    int hidden;
} Hud;

typedef struct Scores {
    char pad0[0x24];
    int missed[2];
} Scores;

extern Scores D_0038DB60;

extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern int func_00254D68(char *, const char *, ...);
extern int func_0023DED0(void);
extern void func_0017B5C8(Hud *, const char *, float, float);

void func_00179FA8(Hud *hud)
{
    char buf[32];

    if (hud->hidden == 0) {
        func_0020FB80(0, 0xF0, 0xC0, -0x52, -0x20);
        func_0020F588(hud->player, 0, 0x6E, 0x6E, 0xC8, 0xAA);
        func_00254D68(buf, "Missed:%d", D_0038DB60.missed[hud->player]);
        if (func_0023DED0() == 2) {
            func_0017B5C8(hud, "Gates", 0.06f, 0.4f);
            func_0017B5C8(hud, buf, 0.06f, 0.52f);
        } else {
            func_0017B5C8(hud, buf, 0.1f, 0.48f);
            func_0017B5C8(hud, "Gates", 0.1f, 0.4f);
        }
    }
}
