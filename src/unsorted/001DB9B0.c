/* UNIT 0x001DB9B0 0x001DB9E0 */
extern int D_004B2828[];
int func_001DB9B0(int flags) {
    if (D_004B2828[0] != 1) {
        return (flags & 1) ? 0x5A : 0x10E;
    }
    return 0;
}
