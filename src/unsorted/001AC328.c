/* UNIT 0x001AC328 0x001AC350 */
struct Unk001AC328 {
    char pad0[0x10];
    void *x10;
};

extern void func_00218498(void *p, int value);

void func_001AC328(struct Unk001AC328 *p, int value)
{
    if (p->x10 != 0) {
        func_00218498(p->x10, value);
    }
}
