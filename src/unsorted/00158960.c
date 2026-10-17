/* UNIT 0x00158960 0x001589A8 */
int func_00158960(unsigned int *arg0) {
    unsigned int value = *arg0;
    unsigned int masked;
    int result = 1;
    if ((value >> 18) - 0x1E46 < 0x29U ||
        (masked = value & 0xFFFC0000) == 0x75300000) {
        result = 0;
    } else if (masked == 0x75440000) {
        result = 0;
    }
    return result;
}
