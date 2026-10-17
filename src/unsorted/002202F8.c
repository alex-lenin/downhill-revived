/* UNIT 0x002202F8 0x002204A8 .rodata=0x298DF0:0x298E20 */
extern unsigned char D_0029E2A0[];
extern char D_00660B80[];

static inline void SetBit(int offset, int bit)
{
    char *p = D_00660B80;
    p += 2;
    p = (char *)((int)p + offset);
    *(unsigned short *)p |= bit;
}

void func_002202F8(int index)
{
    int offset;
    unsigned int i;

    if (D_0029E2A0[index]) {
        offset = index << 5;
        i = 0;
        do {
            char *p = D_00660B80;
            int n;
            char *q;
            p += 8;
            n = i + offset;
            q = (char *)(n + (int)p);
            if (*q && !((unsigned char)*q & 0xE0)) {
                *q = 0;
                switch (i) {
                case 8:  SetBit(offset, 1024);  break;
                case 10: SetBit(offset, 256);   break;
                case 9:  SetBit(offset, 2048);  break;
                case 11: SetBit(offset, 512);   break;
                case 6:  SetBit(offset, 16384); break;
                case 7:  SetBit(offset, 32768); break;
                case 5:  SetBit(offset, 8192);  break;
                case 4:  SetBit(offset, 4096);  break;
                case 1:  SetBit(offset, 128);   break;
                case 0:  SetBit(offset, 32);    break;
                case 2:  SetBit(offset, 16);    break;
                case 3:  SetBit(offset, 64);    break;
                }
            }
            i++;
        } while ((int)i < 12);
    }
}
