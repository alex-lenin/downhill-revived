/* UNIT 0x001A1E88 0x001A2230 */
typedef struct Trick {
    int id;
    int styled;
    int group;
} Trick;

typedef struct Player {
    char pad[0x694];
} Player;

extern Player D_004225A0[];
extern int D_003F52F0[];
extern char *D_00272428[];
extern char *D_0026ED40[];
extern char D_0028E708[];
extern char D_0028E718[];
extern char D_0028E730[];
extern char D_0028E740[];
extern char D_0029D248[];
extern char D_0029D2C8[];
extern char D_0029D2D0[];
extern char D_0029D2D8[];

extern void func_00194E10(Player *player, Trick **tricks, int *count, int *total);
extern void func_0023D650(int font, int *width, int *height);
extern void func_0020F518(int a, int b, int c, int d, int e);
extern void func_0020FB80(int a, int b, int c, int d, int e);
extern void func_0020F9F0(int a, int font, int x, int y, char *text);
extern void func_0020F9A8(int a, int font, int x, int y, char *text);
extern int func_00254D68(char *buf, char *fmt, ...);

void func_001A1E88(int unused, int index) {
    char text[80];
    int groups[8];
    Trick *tricks;
    int count;
    int total;
    int width;
    int height;
    int center;
    int font;
    int line;
    int lines;
    int y;
    int i;

    font = D_003F52F0[0];
    func_00194E10(&D_004225A0[index], &tricks, &count, &total);
    line = 1;
    lines = 0;
    y = 0x26;
    for (i = 7; i >= 0; i--) groups[i] = 0;
    func_0023D650(font, &width, &height);
    center = width / 2;
    func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
    func_0020FB80(0, 0x180, 0x120, -0x8B, -0x20);
    func_0020F9F0(0, font, center, 0x14, D_0028E708);
    func_0020FB80(0, 0x130, 0xE0, -0x83, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    for (i = 0; i < count; i++) {
        if (tricks[i].id == 0x2E) groups[tricks[i].group]++;
        else lines++;
    }
    for (i = 0; i < 8; i++) if (groups[i] > 0) lines++;
    for (i = 0; i < 8; i++) {
        if (groups[i] > 0) {
            if (line < lines) func_00254D68(text, D_0029D2C8, D_00272428[i], groups[i] * 360);
            else func_00254D68(text, D_0029D2D0, D_00272428[i], groups[i] * 360);
            func_0020F9A8(0, font, 0x28, y, text);
            line++;
            y += 10;
        }
    }
    for (i = 0; i < count; i++) {
        if (tricks[i].id != 0x2E) {
            if (line < lines) {
                if (tricks[i].styled) func_00254D68(text, D_0028E718, D_0026ED40[tricks[i].id]);
                else func_00254D68(text, D_0029D2D8, D_0026ED40[tricks[i].id]);
            } else {
                if (tricks[i].styled) func_00254D68(text, D_0028E730, D_0026ED40[tricks[i].id]);
                else func_00254D68(text, D_0029D248, D_0026ED40[tricks[i].id]);
            }
            func_0020F9A8(0, font, 0x28, y, text);
            line++;
            y += 10;
        }
    }
    y += 2;
    func_0020FB80(0, 0x160, 0x100, -0x8B, -0x20);
    func_00254D68(text, D_0028E740, total);
    func_0020F9F0(0, font, center, y, text);
}
