/* UNIT 0x00133CB0 0x00133D68 */
typedef struct Packet {
    unsigned long x0;
    char pad8[0x8];
    char x10[1];
} Packet;

typedef struct Ctx {
    char pad0[0x4];
    char *x4;
    Packet *x8;
    char padC[0x1D4 - 0xC];
    int x1D4;
    char pad1D8[0x4];
    int x1DC;
} Ctx;

extern void func_00133D68(Ctx *ctx);
extern void func_00133DE0(Ctx *ctx);
extern void func_00231A08(void *dst, void *src);

#define BIT57 ((unsigned long)1 << 57)

void func_00133CB0(Ctx *ctx)
{
    unsigned long saved;

    ctx->x1D4++;
    ctx->x1DC++;
    /* clear bit 57 while drawing, restore afterwards */
    saved = (ctx->x8->x0 >> 57) & 1;
    ctx->x8->x0 &= ~BIT57;
    func_00133D68(ctx);
    func_00133DE0(ctx);
    func_00231A08(ctx->x4 + 0x40C0, ctx->x8->x10);
    ctx->x8->x0 = (ctx->x8->x0 & ~BIT57) | (saved << 57);
}
