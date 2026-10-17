/* UNIT 0x0019DE80 0x0019E3A0 */
#include "ui_region.h"
#if defined(__mips__)
#define CallShared0019C868_0 func_0019C868
#else
#define CallShared0019C868_0 Forward0019C868_1
#endif
#include "menu_items.h"
#include "ui_style.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Layout {
    short x0;
    short x2;
    char pad4[3];
    unsigned char x7;
} Layout;

typedef struct Obj {
    char pad0[0x438];
    Layout x438;
    char pad440[0x1C];
    int x45C;
    char pad460[0x48];
    int x4A8;
    char pad4AC[0x164];
    int x610;
    int x614;
    int x618;
    unsigned char x61C[9];
    unsigned char x625;
} Obj;

typedef struct Game {
    int x0;
    char pad4[0x1C];
    int x20;
    char pad24[0x40];
    char x64;
    char pad65[0xEB];
    Obj *x150;
} Game;

typedef struct State {
    int x0;
    char pad4[0x1C];
    unsigned char x20;
} State;

typedef struct TextData {
    char pad0[0xFC];
    char *xFC;
} TextData;

extern void *D_0029DE30;
extern char D_0036B0E0[];
extern char D_0036B100[];
extern State D_003EB760;
extern char D_003F52D0[];
extern char D_0043DD38[];
extern char D_004B2A90[];

extern void func_001873F8(Obj *);
extern void func_00187548(Obj *);
extern int func_001875D0(Obj *);
extern void func_001881C0(Obj *);
extern void func_0018E638(Obj *, int, void *);
extern void func_0018E6B8(Obj *, int, void *);
extern int func_00190D60(void *, int, int);
extern void func_0019C868(void *, int, float, float, float, float, int, int, int);
extern void func_001B5AF0(void *, int, int);
extern void func_001C57A8(void *, u128);
extern void func_0020F9F0(int, int, int, int, char *);
extern int func_0021F818(void);
extern void func_0023D650(int, int *, int *);

static inline int minimum(int a, int b)
{
    return a <= b ? a : b;
}

void func_0019DE80(Obj *obj)
{
    int width;
    int height;
    int temp_w;
    int temp_h;
    int step_w;
    int next_row;
    unsigned char *grid;
    int step_h;
    int row;
    int col;
    int x;
    int index;
    int y;
    int selected;
    int disabled;
    int temp_col;
    int temp_row;
    int temp_col_dec;
    int temp_row_dec;
    unsigned char *flag;
    Layout *layout;
    char *input2;
    Game *game;

    func_001881C0(obj);
    if (D_003EB760.x0 != 0) {
        func_001875D0(obj);
        obj->x625 = 1;
        return;
    }
    if (func_001875D0(obj) == 0) {
        obj->x625 = 1;
        return;
    }

    func_0023D650(((Game *)D_003F52D0)->x20, &width, &height);
    height *= 2;

    if (D_003EB760.x20 == 0 && obj->x618 == 0) {
        func_001873F8(obj);
        func_001873C0((struct SharedMenu *)(void *)(obj), 0x2D, 0);
        func_001873C0((struct SharedMenu *)(void *)(obj), 0x39, 0);
        func_001873C0((struct SharedMenu *)(void *)(obj), func_0021F818() + 0x10, 0);
        func_0018E6B8(obj, 0x1A, &D_0029DE30);
        func_0018E638(obj, 0, &D_0029DE30);
    } else {
        grid = obj->x61C;
        row = 0;
        func_0018E638(obj, (D_003EB760.x20 ^ 1) + 0x40, &D_0029DE30);

        layout = &obj->x438;
        layout->x0 = width / 2;
        layout->x2 = 0x14;
        layout->x7 = 1;
        func_001873F8(obj);

        temp_w = (width - 0x3C) / 3;
        temp_h = (height - 0x8C) / 3;
        step_w = temp_w + 0xA;
        step_h = temp_h + 0xA;
        do {
            next_row = row + 1;
            col = 0;
            y = 0x50;
            flag = (unsigned char *)(row + (unsigned int)grid);
            index = row;
            for (col = 0; col < 3; col++) {
                x = row * step_w + 0x14;
                disabled = *flag ^ 1;
                selected = row == obj->x610 && col == obj->x614;
                CallShared0019C868_0(
                    D_0043DD38,
                    index,
                    (float)x,
                    (float)y,
                    (float)temp_w,
                    (float)temp_h,
                    selected * 0x50 + 0x30,
                    disabled ^ 1,
                    selected ^ 1);
                if (selected != 0) {
                    func_0018D800((const UiStyle *)(void *)(D_0036B100));
                } else {
                    func_0018D800((const UiStyle *)(void *)(D_0036B0E0));
                }
                if (disabled != 0) {
                    func_0020F9F0(
                        0,
                        ((Game *)D_003F52D0)->x20,
                        x + 0x46,
                        y / 2 + 0x14,
                        ((TextData *)D_0029DE30)->xFC);
                }
                flag += 3;
                y += step_h;
                index += 3;
            }
            row = next_row;
        } while (row < 3);
        func_0018E6B8(obj, 0x1B, &D_0029DE30);
    }

    func_001B5AF0(D_003F52D0, 5, 1);
    obj->x4A8 = 5;
    func_00187548(obj);

    if (((Game *)D_003F52D0)->x150 == obj) {
        if (func_00190D60(D_003F52D0 + 0x64, 0, obj->x45C) != 0) {
            if (obj->x614 - 1 >= 0) {
                func_001C57A8(D_004B2A90, (u128)0x5458454E5F53ULL);
            }
            temp_col_dec = obj->x614 - 1;
            obj->x614 = (temp_col_dec <= -1) ? 0 : temp_col_dec;
        } else if (func_00190D60(D_003F52D0 + 0x64, 1, obj->x45C) != 0) {
            if (obj->x614 + 1 < 3) {
                func_001C57A8(D_004B2A90, (u128)0x5458454E5F53ULL);
            }
            temp_col = obj->x614 + 1;
            obj->x614 = minimum(temp_col, 2);
        }

        game = (Game *)D_003F52D0;
        input2 = &game->x64;
        if (func_00190D60(input2, 2, obj->x45C) != 0) {
            if (obj->x610 - 1 >= 0) {
                func_001C57A8(D_004B2A90, (u128)0x5458454E5F53ULL);
            }
            temp_row_dec = obj->x610 - 1;
            obj->x610 = (temp_row_dec <= -1) ? 0 : temp_row_dec;
        } else if (func_00190D60(input2, 3, obj->x45C) != 0) {
            if (obj->x610 + 1 < 3) {
                func_001C57A8(D_004B2A90, (u128)0x5458454E5F53ULL);
            }
            temp_row = obj->x610 + 1;
            obj->x610 = minimum(temp_row, 2);
        }
    }
}
