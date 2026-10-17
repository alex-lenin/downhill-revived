/* UNIT 0x0021B020 0x0021B2C8 .sbss=0x29E210:0x29E214 */
typedef unsigned long u64;

typedef struct {
    unsigned int x0;
    int x4;
    union {
        u64 bits;
        struct { unsigned short x8, xA, xC, xE; } half;
    } x8;
} Packet;

typedef struct {
    char pad[0x44];
    unsigned char x44;
    char pad2[3];
} Entry;

typedef struct {
    Entry entries[80];
    char pad[16];
    unsigned int x1690;
} Pool;

extern Pool D_004D1A18, D_004D30B0;
Packet *D_0029E210 __attribute__((section(".sbss"))) = 0;
extern int func_0020D190(Pool *, void *);
extern unsigned int func_0020D200(Pool *, int);
extern void func_0020D220(Pool *, int, unsigned short);

int func_0021B020(Packet *arg0, int arg1) {
    unsigned int top1;
    unsigned int offset1;
    int offset2;
    unsigned int top2;
    int id, kind, sizeA, sizeB, widthA, widthB;
    Entry *entry;
    unsigned int endA, endB;
    u64 bits, bitsA, bitsB;
    Packet *p, *old;

    top2 = top1 = offset1 = offset2 = 0;
    p = arg0 + arg0->x4;
    id = func_0020D190(&D_004D30B0, arg0);
    entry = (Entry *)((char *)&D_004D30B0 + id * 72);
    if (entry->x44) return 0;
    {
        entry->x44 = 1;
        func_0020D220(&D_004D1A18, id, (unsigned short)D_004D1A18.x1690);
        func_0020D220(&D_004D30B0, id, (unsigned short)D_004D30B0.x1690);
        if (id) {
            offset1 = func_0020D200(&D_004D30B0, id);
            offset2 = func_0020D200(&D_004D1A18, id) - arg1 / 64;
            top2 = func_0020D200(&D_004D1A18, id);
            top1 = func_0020D200(&D_004D30B0, id);
            D_0029E210->x0 = ((int)((char *)p - (char *)D_0029E210) >> 4) * 4;
        }
        do {
            bits = p->x8.bits;
            bits &= 0x3f00000000UL;
            kind = 0;
            if (bits == 0x1b00000000UL || bits == 0x2400000000UL || bits == 0x2c00000000UL) kind = 1;
            if (kind) {
                p->x8.half.xA += offset1;
                bitsA = p->x8.bits;
                endA = p->x8.half.xA;
                widthA = 1 << ((unsigned int)(bitsA >> 40) & 15);
                sizeA = ((widthA > 63) ? widthA : 64) << ((unsigned int)(bitsA >> 44) & 15);
                endA = (unsigned int)(sizeA / 64) + endA;
                top1 = top1 < endA ? endA : top1;
            } else {
                p->x8.half.xA += (offset2 + arg1 / 64);
                bitsB = p->x8.bits;
                endB = p->x8.half.xA;
                widthB = (1 << ((unsigned int)(bitsB >> 40) & 15)) << ((unsigned int)(bitsB >> 44) & 15);
                sizeB = 64;
                if (widthB > sizeB) sizeB = widthB;
                widthB = sizeB / 64;
                endB += widthB;
                top2 = top2 < endB ? endB : top2;
            }
            old = p;
            p = (Packet *)((char *)p + (p->x0 >> 2) * 16);
        } while (old->x0);
        {
            unsigned int finalB = top2 & 0xffff;
            unsigned int finalA = top1 & 0xffff;
            D_004D1A18.x1690 = finalB;
            D_004D30B0.x1690 = finalA;
        }
        D_0029E210 = p;
    }
    return 0;
}
