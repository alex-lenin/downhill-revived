/* UNIT 0x002421C0 0x00242240 */
extern unsigned char *D_0029E488; /* read cursor */
extern unsigned char *D_0029E490; /* start of buffer */

int func_002421C0(unsigned char *buf)
{
    unsigned char *p;
    unsigned char *extra;
    int value;

    D_0029E488 = buf;
    D_0029E490 = buf;
    if (*(int *)buf != 0x4034549) {
        return 0;
    }
    p = buf + 0x16;
    extra = buf + 0x1A;
    D_0029E488 = p;
    value = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
    D_0029E488 = extra;
    D_0029E488 = extra + (*(unsigned short *)extra + 4);
    return value;
}
