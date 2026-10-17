/* UNIT 0x00164EE8 0x00164FF0 */
typedef unsigned int u128 __attribute__((mode(TI)));

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4;

typedef struct Sub130 {
    int x0;
    int x4;
    int x8;
    char padC[0x4];
    char x10;
} Sub130;

typedef struct TypeInfo {
    char pad0[0x10];
    float x10;
    char pad14[0xAC];
} TypeInfo;

typedef struct Effect {
    char pad0[0x100];
    unsigned long *x100;
    char pad104[0xC];
    Vec4 x110;
    int x120;
    char pad124[0xC];
    Sub130 x130;
    char pad144[0x4C];
    int x190;
    char pad194[0x8];
    unsigned char x19C;
    char pad19D[0x3];
    float x1A0;
    char pad1A4[0x8];
    int x1AC;
    int x1B0;
    int x1B4;
    char pad1B8[0x298];
    char x450[0x4];
    unsigned char x454;
} Effect;

extern TypeInfo D_003960A0[];
extern void func_00164CE8(Effect *, void *);
extern void func_00164A88(Effect *);
extern void func_001669D8(Effect *, float);
extern void func_001CF6F8(void *);

void func_00164EE8(Effect *fx, int type, unsigned long *flags, Vec4 *pos, void *desc)
{
    Sub130 *sub;
    TypeInfo *info;

    func_00164CE8(fx, desc);
    fx->x120 = type;
    fx->x100 = flags;
    fx->x454 = (unsigned char)(*flags >> 57) & 1;
    {
        unsigned long mask = ~(1UL << 57);
        *flags &= mask;
    }
    COPY_VECTOR(fx->x110, pos);
    fx->x190 = 0;
    fx->x1B4 = 0;
    fx->x1AC = 0;
    fx->x1B0 = 0;
    func_00164A88(fx);
    fx->x130.x0 = 0;
    sub = &fx->x130;
    sub->x4 = 0;
    sub->x8 = 0;
    fx->x130.x10 = 0;
    info = &D_003960A0[fx->x120];
    fx->x1A0 = info->x10;
    if (fx->x19C) {
        func_001669D8(fx, 100.0f);
    }
    func_001CF6F8(fx->x450);
}
