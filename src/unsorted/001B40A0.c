/* UNIT 0x001B40A0 0x001B45F8 */
#include "include_asm.h"

#ifdef NONMATCHING
/* Non-matching: D_0029D404 is extern here (retail defines it in this file, $gp-relative) so its
   accesses use lui/%lo (+8 bytes); otherwise sched2/regalloc ties (lui order, v0/v1, a0/v1),
   andi 0xFF vs move on the odd-frame test, D_00398C40+8 base folding; best diff 33/342 words
   (with the .sdata definition) */
typedef unsigned int u32;

typedef struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
} Obj;

typedef struct GameState {
    char pad0[0x10C];
    int x10C;
    char pad110[0x178];
    int x288;
} GameState;

typedef struct Settings {
    char pad0[0x5BE4];
    int x5BE4; /* pause state */
} Settings;

typedef struct SystemState {
    char pad0[0x488];
    int x488;
} SystemState;

typedef struct FrameLocals {
    int x0;  /* passed to the frame begin/end helpers */
    int x4;  /* paused last frame */
    int x8;  /* pause-step requested this frame */
    u32 xC;  /* next frame counter */
    int x10; /* fewer than 3 players */
} FrameLocals;

extern unsigned char D_0043DD38[];
extern char D_0037EA60[];
extern Settings D_00398C40;
extern char D_003B2270[];
extern char D_00435500[];
extern SystemState D_0043EEC0;
extern char D_0043F9F0[];
extern GameState D_004B25A0;
extern char D_004B2914[];
extern char D_00513B00[];
extern int D_0029D404;

extern int func_00148560(void *, int);
extern void func_0016C0C8(void *, int);
extern void func_0016C420(void *);
extern void func_00176A80(void *);
extern void func_00176ED8(void *);
extern void func_0017B7A8(void *);
extern void func_00196DF0(void *);
extern void func_0019ED18(void *);
extern void func_001A9640(void *);
extern void func_001B4618(Obj *);
extern int func_001B4688(Obj *);
extern void func_001B4718(Obj *);
extern void func_001B4900(void);
extern void func_001BC4F0(void *);
extern void func_001C6A80(void *);
extern void func_001D4288(void *);
extern void func_001DEE20(void *);
extern void func_002013F8(void);
extern void func_002065C0(void);
extern void func_0020E818(void);
extern void func_0020F2A8(int);
extern void func_002102A8(void *, int, int);
extern void func_00210460(void *, int, int);
extern void func_00215B68(int, int, int, int);
extern void func_002161B8(int, int, int, int);
extern void func_002184D0(int, int, int);
extern void func_00219EC8(void *);
extern void func_0021F1B8(void *, int, int, int);
extern void func_0021F210(int, void *, u32, int, int);
extern int func_0021F3F8(void);
extern void func_0021F8E0(void);
extern int func_00220618(int, int);
extern void func_00228328(int);
extern void func_0022DE40(void);
extern void func_0022E3D0(void);
extern void func_00238B20(void);
extern void func_00238B40(void);
extern void func_00238BD8(void);
extern void func_00238C00(void *);
extern void func_00238CB0(void *);
extern void func_00238D98(void *);
extern void func_002398C0(int, int, int);
extern void func_0023CB00(int);
extern char *func_0023D5F0(int);
extern int func_0023D608(void);
extern void func_0023DF90(int);
extern void func_002433A8(int, int);
extern int func_00243CE8(int);
extern void func_0025A960(int);

static inline int PauseState(Settings *settings)
{
    return settings->x5BE4;
}

/* Main race-frame loop: runs until func_001B4688 reports the race is done,
   returns arg0->xC. */
int func_001B40A0(Obj *arg0)
{
    FrameLocals frame;
    int prev;
    int i;
    int oddFrame;
    int numPlayers;
    char *buffer;
    int i2;
    int i3;
    int i4;
    int playerOffset;
    u32 frameCount;

    frame.x4 = 0;
    numPlayers = func_0023D608();
    if (D_0043DD38[0] == 0) {
        arg0->x4 = 0;
    }
    D_0043DD38[0] = 0;
    frameCount = 0;
    frame.x0 = func_00243CE8(0) == 0;
    func_00238B20();
    func_00238BD8();
    func_0020E818();
    func_0021F8E0();
    while (func_001B4688(arg0) == 0) {
        int paused = PauseState(&D_00398C40);
        frame.x8 = 0;
        if (paused != 0) {
            func_00148560(D_0037EA60, 5);
            frame.x4 = 1;
            frame.x8 = func_00220618(2, 0) != 0;
        } else if (frame.x4 != 0) {
            frame.x4 = 0;
            func_00148560(D_0037EA60, 1);
        }
        i = frameCount & 1; /* i doubles as the odd-frame temp */
        buffer = func_0023D5F0(0);
        func_00219EC8(i ? (buffer + 0x10) : (buffer + 0x50));
        oddFrame = i;
        func_0025A960(0);
        if (frameCount >= (u32)D_0029D404) {
            func_0021F1B8(buffer, oddFrame, 0, 1);
            D_0029D404 = 6;
        }
        if (func_0021F3F8() != 0) {
            func_0021F210(0, buffer, frameCount, frame.x0, numPlayers);
        } else {
            func_0021F210(0, buffer, frameCount, 0, numPlayers);
        }
        i = 0;
        func_0023DF90(frame.x0);
        if (numPlayers > 0) {
            GameState *state = &D_004B25A0;
            char *player;
            playerOffset = 0x150;
            do {
                func_0023CB00(i);
                do {
                    if (i >= state->x288) goto first_next;
                } while (0);
                if ((u32)i >= 4U) goto first_next;
                player = D_003B2270;
                func_00176A80(player + playerOffset);
                player = 0;
first_next:
                i += 1;
                playerOffset += 0x1380;
            } while (i < numPlayers);
        }
        func_0022E3D0();
        if ((u32)D_0029D404 < frameCount) {
            func_00215B68(0, numPlayers, (frameCount ^ 1) & 1, frame.x0);
        }
        func_002013F8();
        func_002102A8(D_00513B00, numPlayers, (frameCount ^ 1) & 1);
        func_0021F8E0();
        /* retail tests the low byte of oddFrame (0 or 1) */
        if (*(unsigned char *)&oddFrame && (func_001B4900(), (func_001B4688(arg0) != 0))) {
            func_001B4618(arg0);
            break;
        } else {
            if (PauseState(&D_00398C40) != 0) {
                if (frame.x8 == 0) {
                    func_0016C0C8(&D_00398C40, 1);
                } else {
                    goto unpause;
                }
            } else {
unpause:
                func_0016C0C8(&D_00398C40, 0);
            }
            i2 = 0;
            frame.x10 = numPlayers < 3;
            frame.xC = frameCount + 1;
            if (numPlayers > 0) {
                do {
                    func_002184D0(0, 0, i2);
                    prev = i2;
                    i2 += 1;
                    func_002398C0(prev, oddFrame, frame.x0);
                } while (i2 < numPlayers);
            }
            if ((PauseState(&D_00398C40) == 0) || (frame.x8 != 0)) {
                func_002065C0();
            }
            func_001DEE20((char *)&D_00398C40 + 8);
            func_001BC4F0((char *)&D_00398C40 + 0x5350);
            func_001D4288((char *)&D_00398C40 + 0x240);
            func_001C6A80((char *)&D_004B25A0 + 0x374); /* == D_004B2914 */
            if (D_004B25A0.x10C == 0xE) {
                i3 = 0;
                if ((u32)(D_0043EEC0.x488 - 2) < 3U) {
                    func_00196DF0(D_00435500);
                    goto second_loop;
                }
            } else {
                i3 = 0;
                if (D_004B25A0.x10C == 0xF) {
                    func_001B4718(arg0);
second_loop:
                    i3 = 0;
                }
            }
            if (numPlayers > 0) {
                GameState *state = &D_004B25A0;
                char *player;
                playerOffset = 0x150;
                do {
                    do {
                        if (i3 >= state->x288) goto second_next;
                    } while (0);
                    if ((u32)i3 < 4U) {
                        player = D_003B2270;
                        func_00176ED8(player + playerOffset);
                        player = 0;
                    }
                    func_0020F2A8(i3);
second_next:
                    i3 += 1;
                    playerOffset += 0x1380;
                } while (i3 < numPlayers);
            }
            if (frame.x10 == 0) {
                func_0017B7A8(D_003B2270);
            }
            func_001B4618(arg0);
            if ((u32)D_0029D404 < frameCount) {
                func_002161B8(0, numPlayers, (frameCount ^ 1) & 1, frame.x0);
            }
            func_002433A8(0, 0);
            i4 = 0;
            func_00210460(D_00513B00, 0, numPlayers);
            func_0016C420(&D_00398C40);
            if (numPlayers > 0) {
                do {
                    func_00228328(i4);
                    i4 += 1;
                } while (i4 < numPlayers);
            }
            if ((PauseState(&D_00398C40) == 0) || (frame.x8 != 0)) {
                func_0022DE40();
            }
            func_002433A8(0, 0);
            frameCount = frame.xC;
            func_002433A8(0, 0);
            if ((PauseState(&D_00398C40) != 0) && (frame.x8 == 0)) {
                func_00238CB0(&frame.x0);
            } else {
                func_00238B40();
                func_00238C00(&frame.x0);
            }
            func_001A9640(D_0043F9F0);
            func_00238D98(&frame.x0);
            func_0019ED18(&D_0043EEC0);
        }
    }
    return arg0->xC;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_001B40A0);
#endif
