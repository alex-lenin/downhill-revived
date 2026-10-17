/* UNIT 0x001E7260 0x001E72A8 .sdata=0x29D750:0x29D754 */
int D_0029D750 = 120;
int func_001E7260(void *arg0, int packed) {
    int x = (signed char)packed;
    int y = (signed char)(packed >> 8);
    int flags = (D_0029D750 < x) << 5;
    if (x < -D_0029D750) flags |= 0x80;
    if (D_0029D750 < y) flags |= 0x40;
    if (y < -D_0029D750) flags |= 0x10;
    return flags;
}

