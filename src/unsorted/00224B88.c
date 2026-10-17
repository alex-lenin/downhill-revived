/* UNIT 0x00224B88 0x00224CC8 */
typedef union Qword {
    unsigned long d[2];
    unsigned int w[4];
} Qword;

extern char D_00265C00[];
extern Qword *D_00279AF0[];
extern Qword *D_00678630[];
/* VU microcode labels (VU memory addresses); no symbol sits at address 0, so
   the one at 0 is reached as D_50 - 0x50 */
extern char D_50[];

Qword *func_00224B88(Qword *p, int idx)
{
    Qword *base = p;
    int count;

    p += 2;

    p->d[0] = 0x7100D;
    p->d[1] = 0x47;
    p++;
    p->d[0] = 0x61;
    p->d[1] = 0x14;
    p++;
    p->d[0] = 0x131000000L;
    p->d[1] = 0x4E;
    p++;
    p->d[0] = 0;
    p->d[1] = 8;
    p++;
    count = p - base;
    base->w[0] = (count - 1) | 0x50000000;
    base->w[1] = (unsigned int)D_00265C00;
    base->w[2] = 0x11000000;
    base->w[3] = (count - 1) | 0x50000000;
    base[1].d[0] = (count - 2) | 0x8000 | (0x8000L << 45);
    base[1].d[1] = 0xE;
    p->w[0] = 0x10000007;
    p->w[1] = 0;
    p->w[2] = 0;
    p->w[3] = 0x6C040000;
    p++;
    D_00279AF0[idx] = p;
    p += 4;
    p->w[0] = 0;
    p->w[1] = 0;
    p->w[2] = 0;
    p->w[3] = 0x6C010007;
    p++;
    D_00678630[idx] = p;
    p++;
    p->w[0] = 0;
    p->w[1] = 0;
    p->w[2] = 0;
    p->w[3] = ((unsigned int)(D_50 - 0x50) >> 3) | 0x15000000;
    return p + 1;
}
