/* UNIT 0x0020F9A8 0x0020F9F0 */
typedef struct { char pad[0x10]; short x10, x12, x14; char pad16[6]; int x1C; } Entry;
extern Entry D_004F3570[];
extern void func_0020FBB0(int, int, int);
void func_0020F9A8(int index, int arg1, int width, int height, int arg4) {
    Entry *entry = &D_004F3570[index];
    int scaled = width << 4;
    entry->x1C = 0;
    entry->x12 = height << 4;
    entry->x14 = scaled;
    entry->x10 = scaled;
    func_0020FBB0(index, arg1, arg4);
}

