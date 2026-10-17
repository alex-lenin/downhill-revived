/* UNIT 0x0016CC38 0x0016CC98 */
void func_0016CC38(char *obj, long *packet) {
    long bits = *packet;
    unsigned int index;
    if ((bits & 0x3F) == 4) {
        index = ((int)(bits >> 18) & 0x3FFF) - 0x50D;
        if (index < 4U) {
            *(long **)((char *)(((int)(bits >> 7) & 0x7FF) * 0x6D0 + (int)obj + (index << 5)) + 0x1B0) = packet;
        }
    }
}
