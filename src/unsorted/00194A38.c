/* UNIT 0x00194A38 0x00194A60 */
char *func_00194A38(char *base, int index, int group) {
    base = group * 0x50 + base;
    base += 0x11948;
    return base + index * 0xC;
}
