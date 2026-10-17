/* UNIT 0x0021E820 0x0021E920 */
typedef unsigned int u128 __attribute__((mode(TI)));

extern char D_50[];
extern u128 D_0065FD80[][24];
extern int func_0021E920(u128 *packet, int x, int y, int w, int h);

int func_0021E820(int idx)
{
    u128 *p = D_0065FD80[idx];
    u128 *start = p;
    int count;

    p[0] = 0;
    ((unsigned int *)p)[2] = ((unsigned int)D_50 >> 3) | 0x15000000;
    p++;
    p += func_0021E920(p, 0, 0, 0x40, 1);
    p += func_0021E920(p, 0, 0, 1, 0x40);
    p += func_0021E920(p, 0, 0x3F, 0x40, 1);
    p += func_0021E920(p, 0x3F, 0, 1, 0x40);
    count = p - start;
    ((unsigned int *)start)[3] = (count - 1) | 0x50000000;
    return count;
}
