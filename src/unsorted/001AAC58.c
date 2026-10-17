/* UNIT 0x001AAC58 0x001AACA8 */
struct Slot {
    char pad[320];
};

extern unsigned char D_0029E4D8[8];
extern unsigned char D_0029E4E0;
extern struct Slot D_004A5990[8];

struct Slot *func_001AAC58(void) {
    int index;
    if (D_0029E4E0 == 8) return 0;
    index = D_0029E4D8[D_0029E4E0++];
    return &D_004A5990[index];
}
