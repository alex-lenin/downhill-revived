/* UNIT 0x00184470 0x001845A8 */
#include "tween.h"
typedef struct Name {
    int x[4];
} Name;

typedef struct Sub {
    char pad0[0x14];
    Name names[1];
} Sub;

typedef struct Game {
    char pad0[0x11BC];
    int x11BC;
    char pad11C0[0xC4];
    int x1284;
    char pad1288[0x15A];
    unsigned char x13E2;
} Game;

typedef struct Ctx {
    Game *game;
    int x4;
    int sel;
    int xC;
    int x10;
    int x14;
    int x18;
} Ctx;

extern void func_001ACFA0(void *, int);
extern void func_00136030(void *, int);
extern void func_00184848(Ctx *);
extern void func_00203E10(Name *, float);
extern void func_00202E38(Name *, int, int, float);


extern int func_00238F48(void);

static inline Name *Sub_name(Sub *sub, int i)
{
    return &sub->names[i];
}

void func_00184470(Ctx *ctx)
{
    char *game = (char *)ctx->game;
    Sub *sub = (Sub *)(game + 0x1E40);
    int prev;
    int on;
    Name name;

    func_001ACFA0(game + 0x4580, 0);
    *(int *)(game + 0x48B0) = 0;
    func_00136030(game + 0x40C0, 1);
    if (ctx->game->x13E2) {
        on = ctx->game->x1284;
    } else {
        on = ctx->game->x11BC;
    }
    if (on) {
        ctx->xC = 1;
    } else {
        ctx->xC = 0;
    }
    prev = ctx->sel;
    func_00184848(ctx);
    if (ctx->sel && prev != ctx->sel) {
        name = *Sub_name(sub, ctx->sel);
        func_00203E10(&name, 0.0f);
        func_00202E38(Sub_name(sub, ctx->sel), 1, 7, ctx->x18);
    }
    func_001B3170((RiderSecondaryBlend *)((char *)sub + 0x16F0), ctx->x18, 0.25f);
    func_001AE630((RiderBodyBlend *)(game + 0x47F0), ctx->x18, 0.0f);
    ctx->x10 = func_00238F48();
    *(int *)(game + 0x6300) = 60;
}
