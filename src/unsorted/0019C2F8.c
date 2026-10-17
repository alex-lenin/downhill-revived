/* UNIT 0x0019C2F8 0x0019C358 */
float func_0019C2F8(unsigned int *data, int x, int y, int shift, float scale) {
    unsigned int value = data[(y << 9) + x];
    value = (value >> shift) & 0xFF;
    return value * scale;
}
