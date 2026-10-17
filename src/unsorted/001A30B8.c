/* UNIT 0x001A30B8 0x001A3220 */
typedef struct Entry {
    char pad0[0x14];
} Entry;

typedef struct Menu {
    Entry entries[14];
    char pad118[0x10];
    int x128[1];
} Menu;

typedef struct Game {
    char pad0[0x501A8];
    int x501A8;
} Game;

typedef struct State {
    char pad0[0x288];
    int x288;
} State;

extern void *D_003F52F0[];
extern Game D_002AF2C0;
extern State D_004B25A0;

extern void func_0023D650(void *font, int *w, int *h);
extern void func_001A2C58(Menu *menu, void *arg);
extern void func_001A2EF8(Menu *menu, Entry *entry, void *arg, int y, void *font, int num);

void func_001A30B8(Menu *menu, void *arg) {
    int j;
    int count;
    void *font;
    int y;
    int step;
    int size[2];
    int i, n;

    step = 10;
    count = D_002AF2C0.x501A8;
    font = D_003F52F0[0];

    func_0023D650(font, &size[0], &size[1]);
    y = 0;
    size[0] /= 2;
    func_001A2C58(menu, arg);
    for (j = 0; j < 3; j++) {
        if (j < count) {
            y = 42 + j * step;
            func_001A2EF8(menu, &menu->entries[j], arg, y, font, j + 1);
        }
    }
    for (i = 0; i < D_004B25A0.x288; i++) {
        n = menu->x128[i];
        if (n >= 4) {
            y += step;
            func_001A2EF8(menu, &menu->entries[n - 1], arg, y, font, n);
        }
    }
}
