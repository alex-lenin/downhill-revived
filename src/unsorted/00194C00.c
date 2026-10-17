/* UNIT 0x00194C00 0x00194C48 */
struct Slot {
    char pad0[0x32];
    char x32;
    char pad33[0x44 - 0x33];
    int x44;
    int x48;
    int x4C;
};

extern void func_001952A0(struct Slot *slot);

void func_00194C00(struct Slot *slots, int index)
{
    int a, b, c, d;
    a = index * 0x50;
    a += (int)slots;
    ((struct Slot *)a)->x32 = 0;
    b = index * 0x50;
    b += (int)slots;
    ((struct Slot *)b)->x44 = 0;
    c = index * 0x50;
    c += (int)slots;
    ((struct Slot *)c)->x4C = ((struct Slot *)c)->x48 = 0;
    d = index * 0x50;
    d += (int)slots;
    func_001952A0((struct Slot *)d);
}
