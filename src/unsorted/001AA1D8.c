/* UNIT 0x001AA1D8 0x001AA238 */
struct Bits { unsigned int low : 6; unsigned int high : 26; };
typedef struct { char pad[0x1C]; struct Bits *x1C; char pad20[0xA]; short x2A; char pad2C[4]; } Entry;
extern unsigned char D_0029E4D0;
extern Entry D_004A4190[];
void func_001AA1D8(void) {
    Entry *entry;
    int count = 0;
    if (D_0029E4D0 != 0) {
        entry = D_004A4190;
        do {
            struct Bits *flags = entry->x1C;
            count++;
            flags->low = entry->x2A;
            entry++;
        } while (count < (int)D_0029E4D0);
    }
}
