/* UNIT 0x00219B68 0x00219EC8 .rodata=0x298738:0x2987A8 */
typedef struct TexHeader {
    union {
        unsigned int key;   /* top half: heightLog2 << 24 | widthLog2 << 16 */
        struct {
            unsigned char x0;
            unsigned char x1;
            unsigned char widthLog2;
            unsigned char heightLog2;
        } b;
    } u;
    unsigned int size;      /* 0x4, bytes */
} TexHeader;

typedef struct TexRec {
    unsigned int x0;
    int x4;
    unsigned long flags; /* bit 38 = no upload */
    char pad10[0x10];
    char x20[0x14];
    int x34;
    int x38;
} TexRec;

extern void func_00254D08(const char *fmt, ...);

/* Returns the address of the given mip level of a small texture. */
unsigned int func_00219B68(TexHeader *hdr, unsigned int base, int level, TexRec *rec)
{
    unsigned long psm = rec->flags & 0x3F00000000;
    unsigned int addr = base;
    unsigned int key;
    int mip;
    int negative;

    if (psm == 0x1400000000 || psm == 0x1300000000) {
        addr = base + (hdr->size >> 2);
    } else if ((1 << hdr->u.b.widthLog2) >= 64 || hdr->u.b.widthLog2 == hdr->u.b.heightLog2) {
        addr = base + (hdr->size >> 2);
    } else if ((key = hdr->u.key & 0xFFFF0000) == 0x03040000 || key == 0x04050000) {
        addr = base + (hdr->size >> 2);
    } else if (key == 0x03050000) {
        addr = (level & 1) ? base + 0x180 : base + 0x80;
    } else if (key == 0x03060000) {
        mip = level % 4;
        if (mip == 0) {
            addr = base + 0x80;
        } else if (mip == 1) {
            addr = base + 0x180;
        } else if (mip == 2) {
            addr = base + 0x80;
        } else {
            negative = mip != 3;
            if (negative) {
                addr = base;
            } else {
                addr = base + 0x580;
            }
        }
    } else if (key == 0x04030000) {
        addr = (level & 1) ? base + 0xC0 : base + 0x40;
    } else if (key == 0x04060000) {
        addr = (level & 1) ? base + 0x600 : base + 0x200;
    } else if (key == 0x05030000) {
        mip = level % 4;
        if (mip == 0) {
            addr = base + 0x40;
        } else if (mip == 1) {
            addr = base + 0xC0;
        } else if (mip == 2) {
            addr = base + 0x40;
        } else {
            negative = mip != 3;
            if (negative) {
                addr = base;
            } else {
                addr = base + 0x2C0;
            }
        }
    } else if (key == 0x05040000) {
        addr = (level & 1) ? base + 0x300 : base + 0x100;
    } else if (key == 0x06050000) {
        addr = (level & 1) ? base + 0xC00 : base + 0x400;
    } else if (key == 0x07050000) {
        addr = (level & 1) ? base + 0x1C00 : base + 0x400;
    } else {
        addr = base;
        func_00254D08("???????? Dont handle %d X %d texture map ????????? \n",
                      1 << hdr->u.b.widthLog2, 1 << hdr->u.b.heightLog2);
    }
    return addr;
}

typedef struct Entry {
    TexRec *rec;
    unsigned int index : 14;
    unsigned int pages : 6;
    unsigned int bin : 5;
    unsigned int count : 3;
    unsigned int pad2 : 3;
    unsigned int active : 1;
    int x8;
    int xC;
} Entry;

extern Entry D_00621310[];
extern void func_00243D80(void *, int, short, short, int, int, short, short);

void func_00219D88(int id, int width, int height, int fmt, int arg4, TexRec *rec, int count, int bin)
{
    int pages;

    if (width < 65) {
        pages = 1;
    } else {
        pages = width / 64;
    }
    if (!(rec->flags & (0x8000L << 23))) {
        func_00243D80(rec->x20, 0, pages, fmt, 0, 0, width, height);
    }
    /* Debug print compiled out; its string is still in retail .rodata (unreferenced). */
    if (0) {
        func_00254D08("loading tex %d w %d h %d word addr %x memAddr %p \n", id, width, height, fmt, rec->x20);
    }
    D_00621310[id].rec = rec;
    D_00621310[id].pages = pages;
    D_00621310[id].count = count;
    D_00621310[id].bin = bin;
    D_00621310[id].active = 0;
    D_00621310[id].xC = rec->x34;
    rec->x38 = 0x7F;
}
