/* UNIT 0x0023D1A0 0x0023D1C8 */
struct Entry { int x0; int *x4; char pad8[0xE8]; };
extern struct Entry D_00528360[];
extern int D_0029E448;
void func_0023D1A0(int index) {
    struct Entry *entry;
    entry = (struct Entry *)(index * 0xF0 + (char *)D_00528360);
    *entry->x4 = D_0029E448;
}
