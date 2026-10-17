/* UNIT 0x0021E4F0 0x0021E728 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union { u128 all; unsigned int words[4]; unsigned long dwords[2]; } Packet;

extern char D_50[];
extern Packet D_0065F980[][30];
extern int func_0021E728(Packet *p, int x, int y, int u0, int v0, int u1, int v1, int w, int h, int bilinear);

int func_0021E4F0(int idx)
{
    Packet *p = D_0065F980[idx];
    Packet *start = p;
    int count;

    p->all = 0;
    p->words[2] = ((unsigned int)D_50 >> 3) | 0x15000000;
    p++;
    p->dwords[0] = 0x1000000000008006;
    p->dwords[1] = 0xE;
    p++;
    p->dwords[0] = 0x101FE;
    p->dwords[1] = 0x4C;
    p++;
    p->dwords[0] = 0x7E0000007E00;
    p->dwords[1] = 0x18;
    p++;
    p->dwords[0] = 0x3F0000003F0000;
    p->dwords[1] = 0x40;
    p++;
    p->dwords[0] = 0;
    p->dwords[1] = 0x3F;
    p++;
    p->dwords[0] = 0x60;
    p->dwords[1] = 0x14;
    p++;
    p->dwords[0] = 0x598007F80;
    p->dwords[1] = 6;
    p++;
    p += func_0021E728(p, 0, 0, 0, 0, 0x3F, 0x3F, 0x3F, 0x3F, 1);
    p->dwords[0] = 0x1000000000008004;
    p->dwords[1] = 0xE;
    p++;
    p->dwords[0] = 0x101FC;
    p->dwords[1] = 0x4C;
    p++;
    p->dwords[0] = 0x7E0000007E00;
    p->dwords[1] = 0x18;
    p++;
    p->dwords[0] = 0x3F0000003F0000;
    p->dwords[1] = 0x40;
    p++;
    p->dwords[0] = 0x598007FC0;
    p->dwords[1] = 6;
    p++;
    p += func_0021E728(p, 0, 0, 0, 0, 0x3F, 0x3F, 0x3F, 0x3F, 0);
    count = p - start;
    start->words[3] = (count - 1) | 0x50000000;
    return count;
}
