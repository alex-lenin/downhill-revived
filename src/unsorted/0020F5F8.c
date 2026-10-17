/* UNIT 0x0020F5F8 0x0020F648 */
struct Entry { char pad0[8]; short x8; short xA; char padC[0x14]; };
extern struct Entry D_004F3570[];
void func_0020F5F8(int index, int arg1) {
    struct Entry *base;
    struct Entry *p;
    int a;
    if (index >= 2) return;
    if (index < 0) return;
    base = D_004F3570;
    p = base + index;
    a = arg1 << 3;
    p->xA = a;
    p->x8 = (arg1 << 6) / 5;
}
