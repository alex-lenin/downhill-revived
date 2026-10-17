/* UNIT 0x001AC300 0x001AC328 */
struct Unk001AC300 {
    char pad0[0xC];
    void *xC;
};

extern void func_00218498(void *p, int value);

void func_001AC300(struct Unk001AC300 *p, int value)
{
    if (p->xC != 0) {
        func_00218498(p->xC, value);
    }
}
