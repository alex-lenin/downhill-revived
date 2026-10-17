/* UNIT 0x001B0240 0x001B0268 */
struct Unk001B0240 {
    char pad0[0x14];
    int x14;
};

extern void func_001B0268(struct Unk001B0240 *p);

void func_001B0240(struct Unk001B0240 *p, int value)
{
    if (value != 0) {
        p->x14 = value;
    }
    func_001B0268(p);
}
