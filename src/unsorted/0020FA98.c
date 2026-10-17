/* UNIT 0x0020FA98 0x0020FB38 */
typedef struct { char pad[16]; short x10, x12, x14; char pad16[6]; int x1C; } Record;
extern Record D_004F3570[];
extern int func_0020F860(int, int);
extern void func_0020FBB0(int, int, int);
void func_0020FA98(int index, int value, int x, int y, int mode) {
    Record *p = &D_004F3570[index];
    int width;
    p->x1C = 2;
    width = (x << 4) - func_0020F860(index, mode);
    p->x10 = width;
    p->x12 = y << 4;
    p->x14 = x << 4;
    func_0020FBB0(index, value, mode);
}
