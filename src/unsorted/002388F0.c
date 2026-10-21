/* UNIT 0x002388F0 0x00238A68 */
#include "include_asm.h"

#ifdef NONMATCHING
/* Non-matching: jump2 cross-jumps the two "return 0" exits and sched1/2 reorder loads, the -16 and the result sra;
   best diff 37/94 words */
typedef unsigned int u128 __attribute__((mode(TI)));

/* 16-byte texture/GS register record; x4 bits 3..4 are rewritten to 01 */
typedef struct Texture {
    int x0;
    int x4;
    int x8;
    int xC;
} Texture;

extern unsigned char D_0029E3A0[];  /* per-slot "pending" flag */
extern Texture *D_0029E3A8[];
extern Texture *D_0029E3B8[];
extern Texture *D_0029E3C0[];
extern Texture D_0067DF90[];
extern char D_00759790[];           /* per-slot 0x780-byte buffers */
extern int func_0023D608(void);
extern int func_00219EE8(int, Texture *, u128 *);
extern void func_002370E0(void *);
extern void func_002370F0(int);
extern void func_002372A8(void);

int func_002388F0(int index, u128 *packet)
{
    u128 *start;
    unsigned char *pending;
    unsigned int mask;
    int qwords;

    if (func_0023D608() >= 2)
        return 0;
    pending = &D_0029E3A0[index];
    if (*pending != 0) {
        mask = 0xFFFFFFE7;
        D_0029E3B8[index]->x0 = D_0067DF90[18].x0;
        start = packet;
        D_0029E3B8[index]->x4 = (D_0067DF90[18].x4 & mask) | 8;
        packet += func_00219EE8(D_0067DF90[18].x8, D_0029E3B8[index], packet);
        func_002370E0(packet);
        func_002370F0(0x50000000);
        ((int *)packet)[1] = (int)(D_00759790 + index * 0x780);
        func_002372A8();
        --start;
        qwords = packet - start;
        func_00219EE8(0, D_0029E3A8[index], 0);
        D_0029E3A8[index]->x4 = (D_0029E3A8[index]->x4 & mask) | 8;
        D_0029E3C0[index]->x0 = (D_0029E3C0[index]->x0 & 0xFFFFFF00)
                              | ((D_0029E3A8[index]->x0 & 0x3FFF) >> 5);
        *pending = 0;
        return qwords;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_002388F0);
#endif
