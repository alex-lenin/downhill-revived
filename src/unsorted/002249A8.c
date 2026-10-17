/* UNIT 0x002249A8 0x002249E0 */
int func_002249A8(unsigned int flags, int mode) {
    if (flags & 0x100) return mode ? 20 : 16;
    if (flags < 2U) return 12;
    return mode ? 12 : 4;
}
