/* UNIT 0x00194940 0x00194968 */
struct Entry { char pad[0x44]; int x44; };
int func_00194940(char *base, int index) {
    base = (index - 4) * 0x50 + base;
    base += 0x11A88;
    return ((struct Entry *)base)->x44;
}
