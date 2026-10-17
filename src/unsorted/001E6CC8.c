/* UNIT 0x001E6CC8 0x001E6D10 */
__asm__(".extern D_0029D750, 4");

extern int D_0029D750;

int func_001E6CC8(void *arg0, int packed) {
    int x = (signed char)packed;
    int y = (signed char)(packed >> 8);
    int flags = (D_0029D750 < x) << 5;
    if (x < -D_0029D750) flags |= 0x80;
    if (D_0029D750 < y) flags |= 0x40;
    if (y < -D_0029D750) flags |= 0x10;
    return flags;
}
