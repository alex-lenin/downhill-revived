/* UNIT 0x00128840 0x001288B8 */
typedef struct Ctx {
    char pad0[0x11C4];
    int x11C4;
    char pad11C8[0x128C - 0x11C8];
    int x128C;
    char pad1290[0x13E4 - 0x1290];
    unsigned char x13E4;
    char pad13E5[0x7A5C - 0x13E5];
    int x7A5C;
} Ctx;

typedef struct Timer {
    Ctx *ctx;
    char pad4[0x1C];
    int x20;
} Timer;

extern int func_00238F60(void);

int func_00128840(Timer *t)
{
    int result = 0;
    int v;

    t->x20 -= func_00238F60();
    if (t->x20 > 0) {
        return 0;
    }
    t->x20 = 0;
    if (t->ctx->x7A5C == 1) {
        if (t->ctx->x13E4 != 0) {
            v = t->ctx->x128C;
        } else {
            v = t->ctx->x11C4;
        }
        result = v != 0;
    }
    return result;
}
