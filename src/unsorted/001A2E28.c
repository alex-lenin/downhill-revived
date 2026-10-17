/* UNIT 0x001A2E28 0x001A2EF8 */
typedef struct Entry {
    char pad0[0x14];
} Entry;

typedef struct Menu {
    Entry entries[14];
} Menu;

typedef struct Game {
    char pad0[0x501A8];
    int count;
} Game;

extern Game D_002AF2C0;
extern void *D_003F52F0[];

extern void func_0023D650(void *font, int *width, int *height);
extern void func_001A2EF8(Menu *menu, Entry *entry, void *arg, int line, void *font, int index);

int func_001A2E28(Menu *menu, int index, void *arg)
{
    int step = 10;
    int count = D_002AF2C0.count;
    void *font = D_003F52F0[0];
    int size[2];
    int i;

    func_0023D650(font, &size[0], &size[1]);
    i = index - 1;
    size[0] /= 2;
    if (i < count) {
        func_001A2EF8(menu, &menu->entries[i], arg, i * step + 42, font, index);
        return 1;
    }
    return 0;
}
