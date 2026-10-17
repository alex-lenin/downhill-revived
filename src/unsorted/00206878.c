/* UNIT 0x00206878 0x002068E8 */
extern int D_0029E160;
extern unsigned char D_004CD480[];
extern int D_004D0108[];
void func_00206878(void) {
 int i, j;
 unsigned char *p;
 unsigned char *base;
 int *table;
 D_0029E160 = 0;
 base = D_004CD480;
 p = base + 0x2C7F;
 i = 127;
 for (i = 127; i >= 0; --i, --p) *p = i;
 base[0x2C80] = 0;
 table = D_004D0108;
 j = 79;
 do { table[j] = 0; --j; } while (j >= 0);
}
