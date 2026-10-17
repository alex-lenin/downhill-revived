/* UNIT 0x001A1380 0x001A1990 */
#include "format.h"
/* func_001A1380: records/results page for one course (obj->x519 = rows drawn so far).
   D_004225A0 holds 18 per-course Track records (0x694 each) followed by the per-level
   Player records (0xD8 each) at 0x7668. */

typedef struct Track {
    char pad0[0x320];
    int time[33];        /* 0x320: best time, 0x0FFFFFFF = none */
    int tricks[33];      /* 0x3A4: tricks score, 0 = none */
    signed char finish[33]; /* 0x428: finish position, 0x7F = none */
    char pad449[0x694 - 0x449];
} Track;

typedef struct Best {
    int time;       /* +0x00, 0x0FFFFFFF = none */
    short rider;    /* +0x04 */
    short x6;
    int x8;
    char npc;       /* +0x0C, 0x41 = NPC (prints the "-NPC" suffix) */
} Best;

/* Per-level record; its address is D_004225A0 + 0x7668 + level * 0xD8, which the
   compiler keeps as D_004225A0 + level * 0xD8 plus these offsets. */
typedef struct Player {
    char pad0[0x7668];
    int tricks;                 /* 0x7668 */
    char pad766C[0x76C4 - 0x766C];
    Best best;                  /* 0x76C4 */
} Player;

typedef struct Obj {
    char pad0[0x519];
    signed char x519;
} Obj;

extern char D_004225A0[];
extern char *D_0026F060[];
extern char D_0028E648[];
extern char D_0028E660[];
extern char D_0028E678[];
extern char D_0028E688[];
extern char D_0028E698[];
extern char D_0028E6A8[];
extern char D_0029D268[];
extern char D_0029D270[];
extern char D_0029D278[];
extern char D_0029D280[];
extern char D_0029D288[];
extern char D_0029D290[];
extern char D_0029D298[];
extern char D_0029D2A0[];
extern char D_0029D2A8[];
extern char D_0029D2B0[];

extern unsigned int func_00193F38(void *);
extern int func_001F8440(int);
extern char *func_00158650(int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00254D68(char *, char *, ...);


#define TRACK(c) ((Track *)(D_004225A0 + (c) * 0x694))

void func_001A1380(Obj *obj, int course, int count)
{
    char buffer[32];
    char no_time[16];
    char no_tricks[16];
    char no_name[16];
    char no_finish[16];
    int max;
    unsigned int mask;
    int i;
    int y;
    int finish;
    int time;
    int tricks;
    int best_time;
    int score;
    int level;
    Player *player;
    Best *best;

    max = count;
    mask = func_00193F38(D_004225A0);
    func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
    func_0020FB80(0, 0x1C0, 0x140, -0x8B, -0x20);
    if (obj->x519 == 0) {
        func_0020F9F0(0, 0, 0x140, 0x14, D_0028E648);
    } else {
        func_0020F9F0(0, 0, 0x140, 0x14, D_0028E660);
    }
    y = 0x40;
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020FB80(0, 0x140, 0xE0, -0x5D, -0x20);
    func_0020F9A8(0, 0, 0x96, 0x2A, D_0028E678);
    func_0020F9A8(0, 0, 0x1A4, 0x2A, D_0028E688);
    func_0020FB80(0, 0xF0, 0xE0, -0x50, -0x20);
    func_0020F9A8(0, 0, 0xC, 0x2E, D_0028E698);
    func_0020F9A8(0, 0, 0xC, 0x36, D_0029D270);
    func_0020F9A8(0, 0, 0x7E, 0x36, D_0029D278);
    func_0020F9A8(0, 0, 0xD0, 0x36, D_0029D280);
    func_0020F9A8(0, 0, 0x112, 0x36, D_0029D288);
    func_0020F9A8(0, 0, 0x170, 0x36, D_0029D290);
    func_0020F9A8(0, 0, 0x1E0, 0x36, D_0029D280);
    func_0020F9A8(0, 0, 0x226, 0x36, D_0029D288);
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    func_00254D68(no_time, D_0028E6A8);
    func_00254D68(no_tricks, D_0029D298);
    func_00254D68(no_name, D_0029D298);
    func_00254D68(no_finish, D_0029D2A0);
    for (i = obj->x519; obj->x519 < max && func_001F8440(i) >= 0; i++) {
        if (mask & (1 << func_001F8440(i))) {
            level = func_001F8440(i);
            func_0020F9A8(0, 0, 0xA, y, D_0026F060[level]);
            level = func_001F8440(i);
            finish = TRACK(course)->finish[level];
            if (finish != 0x7F) {
                func_00254D68(buffer, D_0029D268, finish);
                func_0020F9A8(0, 0, 0xA0, y, buffer);
            } else {
                func_0020F9A8(0, 0, 0xA0, y, no_finish);
            }
            level = func_001F8440(i);
            time = TRACK(course)->time[level];
            if (time != 0x0FFFFFFF) {
                FormatHmsDiscard(time, (char *)(buffer));
                func_0020F9A8(0, 0, 0xBE, y, buffer);
            } else {
                func_0020F9A8(0, 0, 0xBE, y, no_time);
            }
            level = func_001F8440(i);
            tricks = TRACK(course)->tricks[level];
            if (tricks != 0) {
                func_00254D68(buffer, D_0029D268, tricks);
                func_0020F9A8(0, 0, 0x112, y, buffer);
            } else {
                func_0020F9A8(0, 0, 0x112, y, no_tricks);
            }
            level = func_001F8440(i);
            player = (Player *)(D_004225A0 + level * 0xD8);
            best_time = player->best.time;
            if (best_time != 0x0FFFFFFF) {
                FormatHmsDiscard(best_time, (char *)(buffer));
                func_0020F9A8(0, 0, 0x1D6, y, buffer);
                level = func_001F8440(i);
                player = (Player *)(D_004225A0 + level * 0xD8);
                best = &player->best;
                if (best->npc == 0x41) {
                    func_00254D68(buffer, D_0029D2A8, func_00158650(best->rider), D_0029D2B0);
                    func_0020F9A8(0, 0, 0x164, y, buffer);
                } else {
                    func_0020F9A8(0, 0, 0x164, y, func_00158650(best->rider));
                }
            } else {
                func_0020F9A8(0, 0, 0x1D6, y, no_time);
                func_0020F9A8(0, 0, 0x164, y, no_name);
            }
            level = func_001F8440(i);
            player = (Player *)(D_004225A0 + level * 0xD8);
            score = player->tricks;
            if (score != 0) {
                func_00254D68(buffer, D_0029D268, score);
                func_0020F9A8(0, 0, 0x226, y, buffer);
            } else {
                func_0020F9A8(0, 0, 0x226, y, no_tricks);
            }
            y += 0xA;
            obj->x519 = obj->x519 + 1;
        }
    }
}
