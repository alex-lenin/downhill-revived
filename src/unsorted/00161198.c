/* UNIT 0x00161198 0x00161210 */
typedef struct Ctx {
    char pad0[0x78D0];
    void *x78D0;
} Ctx;

typedef struct Game {
    char pad0[0x50130];
    Ctx *x50130;
} Game;

extern Game D_002AF2C0;

extern void func_00220550(int a);
extern void func_0021F8E0(void);
extern void func_00199CA0(void *p);
extern void func_0018DC20(void *p);

void func_00161198(void *arg)
{
    Ctx *ctx = D_002AF2C0.x50130;
    void *obj;

    if (ctx != 0) {
        obj = ctx->x78D0;
        if (obj != 0) {
            func_00220550(1);
            func_0021F8E0();
            func_00199CA0(obj);
            func_00220550(0);
        }
    }
    func_0018DC20(arg);
}
