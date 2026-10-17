/* UNIT 0x001A46A0 0x001A4938 */
#include "format.h"
typedef struct Rider { char pad0[0x796C]; int x796C; } Rider;
typedef struct Entry { Rider *x0; int x4; char pad8[0xC]; } Entry;
typedef struct Obj { Entry entries[2]; } Obj;
typedef struct Scores { char pad0[0x10]; int x10; char pad14[8]; int x1C[2]; } Scores;
extern int D_003F52F0[];
extern Scores D_0038DB60;
extern char D_0029D2F8[];
extern char D_0029D328[];
extern void func_0023D650(int, int *, int *);
extern void func_001A42D8(void *, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern int func_00254D68(char *, const char *, ...);
extern void func_0020F9A8(int, int, int, int, char *);
extern char *func_00158668(int);

void func_001A46A0(Obj *obj, int index, int alpha)
{
    char text[32];
    int vals[2];
    int width;
    int height;
    int font;
    int total0;
    int total1;
    int sel;
    int y;
    int rowh;
    int *score;
    int *base;
    Scores *sc;
    font = D_003F52F0[0];
    func_0023D650(font, &width, &height);
    width /= 2;
    func_001A42D8(obj, alpha, index + 1);
    total0 = obj->entries[0].x4 + (D_0038DB60.x10 - D_0038DB60.x1C[0]) * 60;
    total1 = obj->entries[1].x4 + (D_0038DB60.x10 - D_0038DB60.x1C[1]) * 60;
    vals[0] = total0;
    vals[1] = total1;
    if (index == 0) {
        sel = total1 < total0;
    } else {
        sel = total1 >= total0;
    }
    rowh = 12;
    func_0020FB80(0, 256, 192, -107, -32);
    sc = &D_0038DB60;
    func_0020F518(0, 88, 152, 73, alpha);
    y = index * rowh + 50;
    func_00254D68(text, D_0029D2F8, index + 1);
    func_0020F9A8(0, font, 30, y, text);
    func_0020F9A8(0, font, 70, y, func_00158668((obj->entries[sel].x0->x796C >> 5) - 1));
    FormatHmsDiscard(obj->entries[sel].x4, (char *)(text));
    func_0020F9A8(0, font, 140, y, text);
    base = &sc->x1C[0];
    score = base + sel;
    func_00254D68(text, D_0029D328, *score, sc->x10);
    func_0020F9A8(0, font, 232, y, text);
    func_0020F518(0, 160, 100, 50, alpha);
    FormatHmsDiscard((sc->x10 - *score) * 60, (char *)(text));
    func_0020F9A8(0, font, 300, y, text);
    func_0020F518(0, 88, 152, 73, alpha);
    FormatHmsDiscard(vals[sel], (char *)(text));
    func_0020F9A8(0, font, 400, y, text);
}
