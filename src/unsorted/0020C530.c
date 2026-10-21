/* UNIT 0x0020C530 0x0020C918 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: sine/height FPRs f2/f3 swapped by local-alloc; extern seed D_0029DA10 is reached via lui/%lo (+2 words);
   best diff 9 aligned rows (built 0x3F0 vs 0x3E8) */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned int u128 __attribute__((mode(TI)));

/* one GS packet quadword */
typedef union Packet {
    u128 q;
    unsigned long d[2];
    u32 w[4];
    float f[4];
    s16 h[8];
} Packet;

typedef union Qword {
    u128 q;
    u32 w[4];
} Qword;

/* rectangle/sprite part of the effect's packet buffer */
typedef struct LateFields {
    u8 pad0[0x60];
    u32 x60, x64, x68, x6C;
    s32 x70, x74;
    u32 x78, x7C;
    s32 x80, x84;
    u32 x88, x8C;
    u8 pad90[0x10];
    Qword xA0;
} LateFields;

extern long D_0029DA10; /* LCG seed, defined in 0020C9F0.c */
extern int func_0023D608(void);
extern Packet *func_00224660(Packet *, u32, u32, u32, u32, u32, u32, u32, u32);

/* Build the effect's GS packet: a rectangle centred on (x2A8, x2AC) of half size (x2A0, x2A4),
   then a fan of 50 randomly jittered points on a rotating spiral. */
void func_0020C530(void *arg0)
{
    u8 *p = (u8 *)arg0;
    u8 *base = p;
    u8 *iter;
    u32 *color;
    LateFields *late;
    int xrad;
    int yrad;
    int xoff;
    int yoff;
    int count;
    int i;
    int one;
    int left;
    int top;
    int right;
    int bottom;
    u32 white;
    u32 header;
    u128 zeroq;
    float x;
    float y;
    float sine;
    float cosine;
    float sn, cs, a, hf;
    int raw;
    struct {
        float sincos[2];
        u8 pad8[8];
    } locals;

    xrad = *(int *)(p + 0x2A0) * 8;
    xoff = *(int *)(p + 0x2A8) * 16;
    yoff = *(int *)(p + 0x2AC) * 16;
    count = *(int *)(p + 0x2A4);
    yrad = count * 8;

    if (func_0023D608() == 2) {
        if (xrad < count * 16) {
            xoff += 0x60;
        } else {
            if (*(s16 *)(p + 0x2B6) != 0) {
                yoff += 0x40;
            } else {
                yoff += 0x20;
            }
        }
    }

    *(u128 *)(p + 0x00) = 0;
    *(u32 *)(p + 0x00) = 0x60000008;
    func_00224660((Packet *)(p + 0x10), 0, 0xE, 1, 0, 0, 0, 0, 3);
    one = 1;
    white = 0xFFFFFF;

    *(u32 *)(p + 0x20) = 0x44;
    *(u32 *)(p + 0x24) = 0x14;
    *(unsigned long *)(p + 0x28) = 0x42;
    *(u32 *)(p + 0x30) = 0x30000;
    *(u32 *)(p + 0x38) = 0x47;
    *(u32 *)(p + 0x40) = 0x01000000;
    *(u32 *)(p + 0x48) = 0x4E;
    *(u32 *)(p + 0x44) = one;
    *(u32 *)(p + 0x34) = 0;
    *(u32 *)(p + 0x3C) = 0;
    *(u32 *)(p + 0x4C) = 0;
    func_00224660((Packet *)(p + 0x50), 0, 0x551, 3, 0, 0x46, 1, 1, one);

    late = (LateFields *)p;
    left = xoff - xrad;
    top = yoff - yrad;
    late->x70 = left;
    right = xoff + xrad;
    bottom = yoff + yrad;
    late->x74 = top;
    late->x80 = right;
    zeroq = 0;
    late->x84 = bottom;
    header = 0x6000001E;
    late->x78 = white;
    late->x88 = white;
    late->x60 = 0;
    late->x64 = 0;
    late->x68 = 0;
    late->x6C = 0;
    late->x7C = 0;
    late->x8C = 0;
    p += 0xA0;
    ((Qword *)(base + 0xA0))->q = zeroq;
    ((Qword *)(base + 0xA0))->w[0] = header;
    func_00224660((Packet *)(base + 0xB0), 0, 0x1510, 4, 1, 0, 0, 0, one);

    *(u32 *)(p + 0x20) = 0x4D;
    *(u32 *)(p + 0x24) = 0;
    *(u32 *)(p + 0x28) = 0xFF1080FF;
    *(u32 *)(p + 0x2C) = 0x3F800000;
    *(s16 *)(p + 0x30) = (s16)xoff;
    *(s16 *)(p + 0x32) = (s16)yoff;
    *(u32 *)(p + 0x34) = white;
    *(u32 *)(p + 0x38) = 0x011080FF;
    *(u32 *)(p + 0x3C) = 0x3F800000;
    func_00224660((Packet *)(base + 0xE0), 0, 5, 1, 1, 0, 0, 1, 0x33);
    p = base + 0xF0;

    /* VU0 microroutine returns sine and cosine in vf14 and vf15 (same helper as 0020C9F0.c,
       including the retail nop after vcallms). */
    do {
        __asm__ volatile (
            "mfc1 $3,%2\n\t"
            "qmtc2.ni $3,$vf14\n\t"
            "vcallms 0x978\n\t"
            "nop\n\t"
            "qmfc2.i $3,$vf14\n\t"
            "mtc1 $3,%0\n\t"
            "qmfc2.ni $3,$vf15\n\t"
            "mtc1 $3,%1"
            : "=&f"(sn), "=&f"(cs)
            : "f"(0.12566371262073517f) : "$3");

        raw = *(int *)(base + 0x2A0);
        hf = (float)*(int *)(base + 0x2A4);
        a = (float)(raw >> 1);
        locals.sincos[0] = sn;
        locals.sincos[1] = cs;
    } while (0);
    y = 0.0f;
    /* x = min((float)(x2A0 >> 1), (float)x2A4) */
    __asm__ volatile ("min.s %0,%1,%2" : "=&f"(x) : "f"(a), "f"(hf));

    cosine = locals.sincos[1];
    sine = locals.sincos[0];

    i = 49;
    iter = p;
    color = (u32 *)(base + 0xF4);
    do {
        long seed;
        union { int bits; float value; } random;
        float rx, ry, newY;

        i--;
        seed = D_0029DA10 * 0x41C64E6DL + 0x3039;
        D_0029DA10 = seed;
        random.bits = (seed & 0x7FFFFF) | 0x3F800000;
        random.value -= 0.75f;
        rx = y * random.value;
        ry = x * random.value;
        newY = y * cosine + x * sine;
        x = x * cosine - y * sine;
        y = newY;

        *(s16 *)(iter + 0) = (s16)(xoff + (int)(rx * 16.0f));
        *(s16 *)(iter + 2) = (s16)(yoff + (int)(ry * 8.0f));
        *color = 0xFFFFFF;
        iter += 8;
        color += 2;
    } while (i >= 0);

    /* close the fan: point 50 repeats point 0 */
    {
        u16 firstX = *(u16 *)(p + 0x00);
        u16 firstY = *(u16 *)(p + 0x02);
        *(u16 *)(p + 0x190) = firstX;
        *(u16 *)(p + 0x192) = firstY;
        *(u32 *)(p + 0x194) = 0xFFFFFF;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0020C530);
#endif
