/* UNIT 0x0023D1C8 0x0023D1F0 */
struct Entry { int x0; int *x4; char pad8[0xE8]; };
extern struct Entry D_00528360[];
extern int D_0029E44C;
void func_0023D1C8(int index) {
    struct Entry *entry;
    entry = (struct Entry *)(index * 0xF0 + (char *)D_00528360);
    *entry->x4 = D_0029E44C;
}
