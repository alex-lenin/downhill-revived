/* UNIT 0x0018DD80 0x0018DF28 */
#include "ui_style.h"
typedef UiStyle Rect;

typedef struct Global {
    char pad0[0x4];
    unsigned char x4;
    char pad5[0x20 - 0x5];
    int x20;
} Global;

typedef struct Res {
    char pad0[0x60];
    int x60;
} Res;

extern Global D_003F52D0;
extern Res *D_0029DE68;
extern char D_003F48A0[];
extern char D_00513B00[];

extern int func_0018DF28(int index, int flag);
extern void func_0023D650(int id, int *w, int *h);
extern int func_001B5368(Global *g);
extern void func_0018D930(int obj, int x, int y, int font);
extern int func_0020F860(int a, int font);
extern void func_00190E48(Global *g, char *text, int x, int y, int w, int h);
extern void func_00190DB0(Global *g, int value);
extern void func_002103D8(char *p, int id, char *text);

int func_0018DD80(int obj, int count)
{
    Rect rect;
    int w;
    int h;
    int done;
    int i;
    int width;

    if (D_003F52D0.x4 == 0) {
        done = 1;
        for (i = 0; i < count; i++) {
            if (func_0018DF28(i, 1)) {
                done = 0;
            }
        }
        if (done) {
            return 0;
        }
        func_0023D650(D_003F52D0.x20, &w, &h);
        w /= 2;
        h /= 2;
        func_0018D7D0((UiStyle *)(void *)(&rect), 0x152, 0xC8, -100, -32, 100, 24, 0, 128);
        func_0018D800((const UiStyle *)(void *)(&rect));
        if ((func_001B5368(&D_003F52D0) & 0x7F) < 0x60) {
            func_0018D930(obj, w, h, D_0029DE68->x60);
        }
        width = (func_0020F860(0, D_0029DE68->x60) >> 4) + 35;
        func_00190E48(&D_003F52D0, D_003F48A0, w - (width >> 1) - 2, h - 3, width, 20);
        func_00190DB0(&D_003F52D0, 90);
        func_002103D8(D_00513B00, D_003F52D0.x20, D_003F48A0);
        return 1;
    }
    return 0;
}
