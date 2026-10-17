/* UNIT 0x001A42D8 0x001A46A0 */
#include "format.h"
typedef struct Obj {
    char pad0[0x138];
    int x138;
    int x13C;
} Obj;

typedef struct State {
    char pad0[0x20];
    int x20;
} State;

typedef struct Text {
    char pad0[0xC4];
    char *xC4;
    char *xC8;
    char *xCC;
    char padD0[0x8];
    char *xD8;
    char padDC[0x15C];
    char *x238;
    char *x23C;
    char *x240;
    char *x244;
    char *x248;
} Text;

/* the best time on a course and who set it */
typedef struct BestTime {
    int time;
    short rider;
    char pad6[0xD8 - 6];
} BestTime;

typedef struct Global {
    char pad0[0x76C4];
    BestTime best[(0x12208 - 0x76C4) / 0xD8];
    char pad[(0x12208 - 0x76C4) % 0xD8];
    int x12208;
} Global;

extern State D_003F52D0;
extern Text *D_0029DE68;
extern Global D_004225A0;
extern int D_004B2840[];
extern char D_0028E750[];
extern char D_0028E760[];
extern char D_0028E7D8[];
extern char D_0029D320[];
extern void func_0023D650(int, int *, int *);
extern void func_0020F648(int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);

extern char *func_00158650(int);
extern int func_00254D68(char *, char *, ...);

void func_001A42D8(Obj *obj, int alpha, int stage)
{
    char text[64];
    char tmp[32];
    int width;
    int height;
    int font;
    int level;
    int offset;
    BestTime *best;

    font = D_003F52D0.x20;
    func_0023D650(font, &width, &height);
    width /= 2;
    func_0020F648(0, 36, 24);
    func_0020F588(font, 0, 200, 200, 0, stage ? 190 : alpha);
    func_0020FB80(0, 384, 288, -155, -32);
    func_0020F9F0(0, font, width, 5, D_0029DE68->x238);
    func_0020FB80(0, 256, 192, -107, -32);
    func_0020F518(0, 110, 110, 200, alpha);
    func_0020F9A8(0, D_003F52D0.x20, 15, 38, D_0029DE68->xC4);
    func_0020F9A8(0, D_003F52D0.x20, 78, 38, D_0029DE68->xC8);
    func_0020F9A8(0, D_003F52D0.x20, 156, 30, D_0029DE68->xD8);
    func_0020F9A8(0, D_003F52D0.x20, 156, 38, D_0029DE68->xCC);
    func_0020F9A8(0, D_003F52D0.x20, 230, 38, D_0029DE68->x23C);
    func_0020F9A8(0, D_003F52D0.x20, 315, 30, D_0029DE68->x240);
    func_0020F9A8(0, D_003F52D0.x20, 300, 38, D_0029DE68->x244);
    func_0020F9A8(0, D_003F52D0.x20, 400, 30, D_0029DE68->x248);
    func_0020F9A8(0, D_003F52D0.x20, 420, 38, D_0029DE68->xCC);
    level = D_004B2840[0];
    offset = level * sizeof(BestTime);
    offset += (int)&D_004225A0;
    best = (BestTime *)(offset + 0x76C4); /* &D_004225A0.best[level] */
    FormatHmsDiscard(D_004225A0.best[level].time, (char *)(tmp));
    func_00254D68(text, D_0028E7D8, tmp, func_00158650(best->rider));
    func_0020F9F0(0, D_003F52D0.x20, width, 80, text);
    if (stage > 0) {
        if (obj->x13C || obj->x138) {
            func_0020F588(font, 0, 160, 100, 50, 140);
            if (obj->x138) {
                func_0020F9A8(0, D_003F52D0.x20, 350, 150, D_0028E760);
                func_0020F9A8(0, D_003F52D0.x20, 350, 160, D_0029D320);
            }
        }
        func_0020FB80(0, 280, 256, -98, -32);
        func_0020F518(0, 221, 219, 10, 140);
        func_00254D68(text, D_0028E750, D_004225A0.x12208 / 20);
        func_0020F9F0(0, D_003F52D0.x20, 256, 133, text);
    }
}
