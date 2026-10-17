/* UNIT 0x00194A10 0x00194A38 */
struct Entry { char pad[0x32]; signed char x32; };
signed char func_00194A10(char *base, int index) {
    base = (index - 4) * 0x50 + base;
    base += 0x11A88;
    return ((struct Entry *)base)->x32;
}
