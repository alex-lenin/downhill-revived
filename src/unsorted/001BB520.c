/* UNIT 0x001BB520 0x001BB5A0 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Packet { u128 q; unsigned long d[2]; unsigned int w[4]; } Packet;
Packet *func_001BB520(Packet *p, int x, int y, int width, int height, unsigned int color) {
    p->d[0] = ((unsigned long)0xC08D << 46) | 0x8001;
    p->d[1] = 0x551;
    p++;
    p->w[0] = 0;
    p->w[1] = 0;
    p->w[2] = 0;
    p->w[3] = color;
    p++;
    p->q = 0;
    p->w[0] = x << 4;
    p->w[1] = y << 4;
    p->w[2] = 0xFFFF28;
    p++;
    p->q = 0;
    p->w[0] = (x + width) << 4;
    p->w[1] = (y + height) << 4;
    p->w[2] = 0xFFFF28;
    return p + 1;
}
