/* UNIT 0x0020F9F0 0x0020FA98 */
struct Entry { char pad[0x10]; short x10, x12, x14; char pad16[6]; int x1c; };
extern struct Entry D_004F3570[];
extern int func_0020F860(int, int);
extern void func_0020FBB0(int, int, int);
void func_0020F9F0(int index, int arg, int x, int y, int extra) {
    struct Entry *p = &D_004F3570[index];
    int left;
    p->x1c = 1;
    left = (x << 4) - (func_0020F860(index, extra) >> 1);
    p->x10 = left;
    p->x12 = y << 4;
    p->x14 = x << 4;
    func_0020FBB0(index, arg, extra);
}
