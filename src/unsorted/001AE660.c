/* UNIT 0x001AE660 0x001AE730 */
struct Channels {
    char pad0[0x12];
    short x12;
    short x14;
    short x16;
};

struct Obj {
    char pad0[0x7C];
    float x7C[3];
    char pad88[0x18];
    float xA0[3];
    char padAC[0x4];
    struct Channels *xB0;
};

struct Ctx {
    char pad0[0x8];
    struct Obj *x8;
    float *xC;
    unsigned char *x10;
};

static inline void SetChannel(struct Ctx *ctx, short *idx, float value)
{
    unsigned int bit;

    ctx->xC[*idx] = value;
    bit = *idx;
    ctx->x10[bit >> 3] |= 1 << (bit & 7);
}

void func_001AE660(struct Ctx *ctx)
{
    struct Obj *obj = ctx->x8;
    struct Channels *ch = obj->xB0;
    float height;

    SetChannel(ctx, &ch->x12, obj->xA0[0]);
    SetChannel(ctx, &ch->x14, obj->xA0[1]);
    height = obj->x7C[2];
    SetChannel(ctx, &ch->x16, obj->xA0[2] + height);
}
