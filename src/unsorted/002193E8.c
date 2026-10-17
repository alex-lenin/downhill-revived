/* UNIT 0x002193E8 0x00219B68 .sdata=0x29DAAC:0x29DACC .rodata=0x298560:0x298738 */
typedef struct Bin {
    unsigned short count;
    unsigned char w;
    unsigned char h;
    unsigned int size;
} Bin;

typedef struct TexList {
    int count;
    int start;
} TexList;

typedef struct TexRec {
    unsigned int x0;
    int x4;
    union {
        unsigned long flags;
        struct {
            unsigned short id;
            unsigned short unkA;
            unsigned int hi;
        } f;
    } u;
} TexRec;

typedef struct TabEnt {
    int x0;
    short x4;
    short x6;
} TabEnt;

typedef struct Big {
    char pad0[0x1680];
    int x1680;
    char pad1684[8];
    int x168C;
    int x1690;
} Big;

extern Big D_004D1A18;
extern Bin D_0062E790[16];
extern int D_0062E810[16];
extern TabEnt D_00625190[16][300];
extern Bin *D_0029E214;
extern unsigned int D_0029E21C;
extern unsigned int D_0029E22C;
extern TexList *func_0020D170(Big *b, int id);
extern void func_002189F8(const char *fmt, ...);
extern void func_00254D08(const char *fmt, ...);
extern int func_0021A008(Bin *out);
extern unsigned int func_00219B68(void *hdr, unsigned int base, int level, TexRec *rec);
extern void func_00219D88(int id, int w, int h, int fmt, int a4, TexRec *rec, int count, int bin);

int D_0029DAAC = 0;
int D_0029DAB0 = 0;
int D_0029DAB4 = 5;
int D_0029DAB8 = 1;
int D_0029DABC = 0;
int D_0029DAC0 = 0;
int D_0029DAC4 = 1;
int D_0029DAC8 = 1;

extern Big D_004D30B0;
extern int D_0029E240;
extern int D_0029E244;
extern unsigned short D_0029E248;
extern int D_0029E24C;
extern unsigned short D_0029E250;

void func_002193E8(void)
{
    D_0029E240 = D_0029DAB0;
    D_0029E244 = D_004D1A18.x168C;
    D_0029E248 = D_004D1A18.x1690;
    D_0029E24C = D_004D30B0.x168C;
    D_0029E250 = D_004D30B0.x1690;
}

void func_00219438(void)
{
    D_0029DAB0 = D_0029E240;
    D_004D1A18.x168C = D_0029E244;
    D_004D1A18.x1690 = D_0029E248;
    D_004D30B0.x168C = D_0029E24C;
    D_004D30B0.x1690 = D_0029E250;
}

extern void func_0020D1E8(Big *, int, int);
extern int func_0021AE70(TexList *, TexRec *);
extern void func_00219D88(int, int, int, int, int, TexRec *, int, int);
extern void func_00254D08(const char *, ...);

void func_00219488(TexRec *recs, int arg1) {
    TexList *ctx;
    TexRec *rec;
    TexRec *prev;
    int base;
    int addr;
    int bin;

    ctx = func_0020D170(&D_004D1A18, 0);
    base = D_004D1A18.x168C;
    rec = recs + ctx->start;
    addr = (unsigned short)rec->u.flags + base;
    do {
        bin = func_0021AE70(ctx, rec);
        if (bin == -1) {
            func_00254D08("Damn,  no bin for dyn texture %d %d \n",
                          1 << ((int)(rec->u.flags >> 40) & 0xF), 1 << ((int)(rec->u.flags >> 44) & 0xF));
        } else {
            addr = (unsigned short)rec->u.flags + base;
            func_00219D88(addr, 1 << ((int)(rec->u.flags >> 40) & 0xF), 1 << ((int)(rec->u.flags >> 44) & 0xF),
                          (int)(rec->u.flags >> 32) & 0x3F, (int)(rec->u.flags >> 51) & 0xF, rec,
                          (int)(rec->u.flags >> 48) & 0x7, bin);
        }
        prev = rec;
        rec += rec->x0 >> 2;
    } while (prev->x0 != 0);
    func_0020D1E8(&D_004D1A18, arg1, D_004D1A18.x168C);
    D_004D1A18.x168C = addr;
}

/* One texture in a packed file: x0 = size, x8 = format bits, image data from x10. */
typedef struct Tex {
    unsigned int x0;
    int x4;
    unsigned long x8;
} Tex;

extern Big D_004D30B0;
extern Big D_004D1A18;


extern void func_0020D220(Big *, int, int);
extern void func_00254D08(const char *, ...);
extern void func_00243D80(void *, short, short, int, short, short, short, short);
extern void func_0025A960(int);
extern void func_002433A8(int, int);
extern void func_002440A8(void *, void *);

unsigned int func_002195F0(char *base, int id, int addr) {
    char image[0x60] __attribute__((aligned(16)));
    Tex *tex;
    Tex *cur;
    unsigned int pos;
    unsigned long psm;
    int resident, width, pages, size;

    tex = (Tex *)(base + func_0020D170(&D_004D30B0, id)->start * 16);
    D_004D1A18.x1690 = (unsigned short)addr;
    func_0020D220(&D_004D1A18, id, (unsigned short)D_004D1A18.x1690);
    pos = addr << 6;
    do {
        psm = tex->x8 & (0x3FL << 32);
        resident = 0;
        if (psm == (0x1BL << 32) || psm == (0x24L << 32) || psm == (0x2CL << 32)) {
            resident = 1;
        }
        if (resident) {
            func_00254D08("Don't support resident texture for dynamic loaded stuff \n");
        } else {
            width = 1 << ((int)(tex->x8 >> 40) & 0xF);
            if (width < 65) {
                pages = 1;
            } else {
                pages = width / 64;
            }
            func_00243D80(image, pos >> 6, pages, (int)((long)tex->x8 >> 32) & 0x3F, 0, 0,
                          1 << ((int)(tex->x8 >> 40) & 0xF), 1 << ((int)(tex->x8 >> 44) & 0xF));
            func_0025A960(0);
            func_002433A8(0, 0);
            func_002440A8(image, &tex[1]);
            func_002433A8(0, 0);
            size = (1 << ((int)(tex->x8 >> 40) & 0xF)) << ((int)(tex->x8 >> 44) & 0xF);
            if ((unsigned int)size < 64) {
                size = 64;
            }
            pos += size;
        }
        cur = tex;
        tex = (Tex *)((char *)tex + (tex->x0 >> 2) * 16);
    } while (cur->x0 != 0);
    D_004D1A18.x1690 = (unsigned short)(pos >> 6);
    return pos >> 6;
}

void func_002197F0(void *tex)
{
    TexList *list;
    TexRec *rec;
    TexRec *prev;
    TabEnt *e;
    Bin *b;
    unsigned int nw;
    int s2;
    int j;
    int flag;

    func_0021A008(D_0062E790);
    D_0029DAAC = 0;
    flag = 1;
    for (s2 = 15; s2 >= 0; s2--) {
        D_0062E810[s2] = 0;
    }
    func_002189F8("We have loaded %i texture files, time to figure out the bins.\n", D_004D1A18.x1680);
    list = func_0020D170(&D_004D1A18, 0);
    D_0029E214 = D_0062E790;
    rec = (TexRec *)((char *)tex + list->start * 16);
    do {
        if (!(rec->u.flags & (0x8000L << 24))) {
            if (flag != 0) {
                s2 = 0;
                flag = 0;
                if (list->count > 0) {
                    do {
                        func_002189F8("New Bin #%i Started (%d blks).  At address 0x%x\n", s2, D_0029E214[s2].count, D_0029E21C);
                        j = 0;
                        b = D_0029E214;
                        {
                            for (; j < b[s2].count; j++) {
                                e = (TabEnt *)((char *)D_00625190 + ((j + D_0062E810[s2]) * 8 + (s2 + D_0029DAAC) * 0x960));
                                e->x0 = 0;
                                e->x4 = 0;
                                e->x6 = D_0029E21C >> 6;
                                nw = func_00219B68(&b[s2], D_0029E21C, j, rec);
                                D_0029E21C = nw;
                                if (D_0029E22C < nw) {
                                    func_00254D08("Texture bin allocation overflow addr %x \n", nw);
                                }
                                b = D_0029E214;
                            }
                        }
                        D_0062E810[s2] = j;
                        if (D_0029E21C & 0x7FF) {
                            D_0029E21C = ((D_0029E21C >> 11) + 1) << 11;
                        }
                        s2++;
                    } while (s2 < list->count);
                }
                D_0029DAAC = s2;
                D_0029DAB0 = 0;
            }
            rec->u.f.unkA = 0;
            D_0029DAB0++;
            if (D_0029DAB0 == 1000) {
                func_00254D08("Texture count at %i\n", 1000);
            }
            func_002189F8("Texture Setup w %d h %d head addr %p sizeD %d texId %d texelF %x bin %d mips %d offset %d \n",
                1 << ((int)(rec->u.flags >> 40) & 0xF),
                1 << ((int)(rec->u.flags >> 44) & 0xF),
                rec,
                (int)(rec->u.flags >> 51) & 0xF,
                rec->u.f.id,
                (int)((long)rec->u.flags >> 32) & 0x3F,
                (int)(rec->u.flags >> 59),
                (int)(rec->u.flags >> 48) & 0x7,
                rec->x0);
            func_00219D88(rec->u.f.id,
                1 << ((int)(rec->u.flags >> 40) & 0xF),
                1 << ((int)(rec->u.flags >> 44) & 0xF),
                (int)((long)rec->u.flags >> 32) & 0x3F,
                (int)(rec->u.flags >> 51) & 0xF,
                rec,
                (int)(rec->u.flags >> 48) & 0x7,
                (int)(rec->u.flags >> 59));
        }
        prev = rec;
        rec += rec->x0 >> 2;
    } while (prev->x0 != 0);
    func_002189F8("Textures done at id number %i (0x%x) because offset is %x, next head is at %p.\n",
        prev->u.f.id, prev->u.f.id, 0, rec);
}
