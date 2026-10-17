/* UNIT 0x001AE730 0x001AE758 */
struct Unk001AE730 {
    char pad0[0x60];
    int x60;
};

extern void func_001AE878(struct Unk001AE730 *p);

void func_001AE730(struct Unk001AE730 *p)
{
    func_001AE878(p);
    p->x60 = 0;
}
