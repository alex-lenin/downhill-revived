/* UNIT 0x001AA140 0x001AA1D8 */
typedef struct Entry {
    char pad0[0x30];
} Entry;

extern unsigned char D_0029E4D0;
extern unsigned char D_0029E4D8[8];
extern unsigned char D_0029E4E0;
extern Entry D_004A4190[];
extern void func_001AAAE8(Entry *entry);

void func_001AA140(void)
{
    int i;
    int j;

    for (i = 0; i < D_0029E4D0; i++) {
        func_001AAAE8(&D_004A4190[i]);
    }
    D_0029E4D0 = 0;
    for (j = 0; j < 8; j++) {
        D_0029E4D8[j] = j;
    }
    D_0029E4E0 = 0;
}
