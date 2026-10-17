/* UNIT 0x001E3060 0x001E30A8 */
struct Obj {
    char pad0[0x500];
    int x500;
};

int func_001E3060(int object, int value)
{
    int i;
    int found = 0;
    int index = ((struct Obj *)object)->x500;
    unsigned char *p;

    object += 0x520;
    p = (unsigned char *)(index * 13 + object);
    for (i = 0; i < 12; i++) {
        if (*p++ == value) {
            found = 1;
        }
    }
    return found;
}
