/* UNIT 0x0018E098 0x0018E1C0 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Menu {
    int cursor;
    char pad4[0x418 - 0x4];
    int count;
    char pad41C[0x45C - 0x41C];
    int x45C;
    char pad460[0x4A0 - 0x460];
    int x4A0;
    int x4A4;
} Menu;

typedef struct Global {
    char pad0[0x64];
    int x64;
} Global;

extern Global D_003F52D0;
extern char D_004B2A90[];

extern void func_0018DFC8(Menu *menu, int dir);
extern int func_00190D60(void *p, int button, int mask);
extern void func_001C57A8(void *p, u128 name);

static inline void MoveCursor(Menu *menu, int dir, u128 sound)
{
    int old = menu->cursor;

    menu->cursor = old + dir;
    func_0018DFC8(menu, dir);
    if (menu->cursor != old) {
        func_001C57A8(D_004B2A90, sound);
    }
}

void func_0018E098(Menu *menu)
{
    Global *g;
    int *pad;

    func_0018DFC8(menu, 1);
    if (menu->count >= 2) {
        g = &D_003F52D0;
        pad = &g->x64;
        if (func_00190D60(pad, menu->x4A4, menu->x45C)) {
            MoveCursor(menu, 1, (u128)0x5458454E5F53UL);
        }
        if (func_00190D60(pad, menu->x4A0, menu->x45C)) {
            MoveCursor(menu, -1, ((u128)0x5355 << 64) | (u128)0x4F49564552505F53UL);
            menu->cursor = menu->cursor > -1 ? menu->cursor : 0;
        }
    }
}
