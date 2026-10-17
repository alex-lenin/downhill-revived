/* UNIT 0x00194A90 0x00194AB8 */
struct Entry { char pad[0x30]; unsigned char x30; };
unsigned char func_00194A90(char *base, int index, int group) {
    base = group * 0x50 + base;
    base += 0x11948;
    base += index;
    return ((struct Entry *)base)->x30;
}
