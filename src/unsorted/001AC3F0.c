/* UNIT 0x001AC3F0 0x001AC418 */
struct Unk001AC3F0 {
    char pad0[0x1054];
    char x1054[0x680];
    int x16D4;
};

extern void func_00163418(void *a, struct Unk001AC3F0 *b);

void func_001AC3F0(struct Unk001AC3F0 *p, int value)
{
    p->x16D4 = value;
    func_00163418(p->x1054, p);
}
