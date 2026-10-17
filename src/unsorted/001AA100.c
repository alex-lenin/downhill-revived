/* UNIT 0x001AA100 0x001AA128 */
struct Unk001AA100 {
    unsigned char initialized;
    char pad1[3];
    int x4;
};

extern int D_0039E834[];

void func_001AA100(struct Unk001AA100 *p)
{
    if (p->initialized == 0) {
        p->initialized = 1;
        p->x4 = D_0039E834[0];
    }
}
