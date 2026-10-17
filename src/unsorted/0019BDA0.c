/* UNIT 0x0019BDA0 0x0019BDD8 */
void func_0019BDA0(char *p, int index, int value) {
    int end = index + 5;
    p += 0x24;
    for (; index < end; index++) p[index] = value;
}
