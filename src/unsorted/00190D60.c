/* UNIT 0x00190D60 0x00190D88 */
struct Entry {
    int x0;
    unsigned int x4;
    unsigned int x8;
};

struct A {
    struct Entry entries[14];
    int xA8;
    unsigned char xAC;
};

unsigned int func_00190D60(struct A *a, int i, unsigned int mask)
{
    if (a->xAC != 0) {
        return 0;
    }
    return a->entries[i].x4 & mask;
}
