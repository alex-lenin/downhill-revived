/* UNIT 0x00227F10 0x00227FD0 */
typedef struct Texture {
    int flags;
    char pad4[0x4];
    unsigned short x8;
    char padA[0x16];
    struct Texture *x20;
    char pad24[0x2C];
    unsigned int tex0_lo;
    unsigned int tex0_hi;
} Texture;

typedef struct TexSlot {
    unsigned int tex0_lo;
    unsigned int tex0_hi;
    int x8;
    unsigned short width;
    unsigned short height;
} TexSlot;

extern Texture *D_0071FC80[];
extern TexSlot D_0067DF90[];

void func_00227F10(int slot, Texture *tex) {
    if ((tex->flags & 0x3F) == 1) {
        tex = tex->x20;
    }
    if (slot < 28) {
        D_0071FC80[slot] = tex;
        tex->flags = (tex->flags & ~0x3F) | 9;
        D_0067DF90[slot].tex0_lo = tex->tex0_lo;
        D_0067DF90[slot].tex0_hi = tex->tex0_hi;
        D_0067DF90[slot].x8 = tex->x8;
        D_0067DF90[slot].width = 16 << ((tex->tex0_lo >> 26) & 0xF);
        D_0067DF90[slot].height = 16 << ((tex->tex0_lo >> 30) | ((tex->tex0_hi << 2) & 0xC));
    }
}
