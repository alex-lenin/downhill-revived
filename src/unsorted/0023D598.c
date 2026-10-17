/* UNIT 0x0023D598 0x0023D5C0 */
struct Entry { long x0; char pad8[0x168]; long x170; char pad178[0x1E8]; };
extern struct Entry D_0075BC70[];
void func_0023D598(int index, long value) {
    D_0075BC70[index].x170 = value;
    *(long *)(index * 0x360 + (char *)D_0075BC70) = value;
}
