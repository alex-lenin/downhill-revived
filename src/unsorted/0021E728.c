/* UNIT 0x0021E728 0x0021E820 */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union { U128 wide; unsigned long halves[2]; unsigned int words[4]; } Packet;

int func_0021E728(Packet *p, int x, int y, int u0, int v0, int u1, int v1, int w, int h, int bilinear) {
    int offset;

    p->halves[0] = ((unsigned long)0xA1168000 << 31) | 0x8001;
    p->halves[1] = 0x53531;
    offset = bilinear ? 0x11 : 1;
    ++p;
    p->words[0] = 0x80; p->words[1] = 0x80; p->words[2] = 0x80; p->words[3] = 0x80;
    ++p;
    x += 0x7E0;
    p->wide = 0;
    p->words[0] = (u0 << 4) + offset;
    p->words[1] = (v0 << 4) + offset;
    y += 0x7E0;
    ++p;
    p->wide = 0;
    p->words[0] = x << 4;
    p->words[1] = y << 4;
    p->words[2] = 0xFFFF28;
    ++p;
    p->wide = 0;
    p->words[0] = (u1 << 4) + offset;
    p->words[1] = (v1 << 4) + offset;
    ++p;
    p->wide = 0;
    p->words[0] = (x + w) << 4;
    p->words[1] = (y + h) << 4;
    p->words[2] = 0xFFFF28;
    return 6;
}
