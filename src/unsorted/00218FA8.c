/* UNIT 0x00218FA8 0x002193E8 .rodata=0x298470:0x298560 */
typedef struct TexEnv {
    char data[0x60];
} TexEnv __attribute__((aligned(16)));

typedef struct TexRec {
    unsigned int x0;
    int x4;
    union {
        unsigned long tex;
        struct {
            unsigned short id;
            unsigned short vram;
            unsigned short xC;
            unsigned short xE;
        } h;
    } u;
    char pad10[0x10];
    TexEnv env;
} TexRec;

#define TEX_ID(r) ((r)->u.h.id)
#define TEX_VRAM(r) ((r)->u.h.vram)
/* psm 0x1B = PSMT8H, 0x24 = PSMT4HL, 0x2C = PSMT4HH (textures that live in CLUT pages) */
#define TEX_PSM(r) ((int)((r)->u.tex >> 32) & 0x3F)
#define TEX_TW(r) ((int)((r)->u.tex >> 40) & 0xF)
#define TEX_TH(r) ((int)((r)->u.tex >> 44) & 0xF)
#define TEX_COUNT(r) ((int)((r)->u.tex >> 48) & 0x7)
#define TEX_LEVELS(r) ((int)((r)->u.tex >> 51) & 0xF)

typedef struct EntryBits {
    unsigned int index : 14;
    unsigned int tbw : 6;
    unsigned int bank : 5;
    unsigned int count : 3;
    unsigned int pad2 : 3;
    unsigned int active : 1;
} EntryBits;

typedef struct Entry {
    TexRec *rec;
    EntryBits x4;
    int x8;
    int xC;
} Entry;

typedef struct TexFile {
    int x0;
    int x4;
} TexFile;

extern char D_004D30B0[];
extern TexEnv D_00620300;
extern Entry D_00621310[];
extern int D_0029E21C;
extern int D_0029E220;

extern void func_00254204(void *, int, int);
extern TexFile *func_0020D170(void *, int);
extern void func_002189F8(char *, ...);
extern void func_00243D80(TexEnv *, short, short, int, int, int, short, short);
extern void func_0025A960(int);
extern int func_002212D8(void);
extern int func_002212A0(void);
extern void func_0025A720(int);
extern void func_0025A700(int);
extern void func_002433A8(int, int);
extern void func_002440A8(TexEnv *, void *);

void func_00218FA8(TexRec *recs) {
    int texBytes;
    int clutBytes;
    int clutCount[2];
    unsigned int top;
    TexRec *rec;
    TexRec *prev;
    int quads;
    int tbw;
    int width;
    unsigned int clutSize;
    unsigned int texSize;
    Entry *entry;
    int id;

    func_00254204(clutCount, 0, 8);
    texBytes = 0;
    clutBytes = 0;
    top = 0;
    rec = (TexRec *)((char *)recs + func_0020D170(D_004D30B0, 0)->x4 * 16);
    do {
        width = 1 << TEX_TW(rec);
        if (width > 64) {
            tbw = width / 64;
        } else {
            tbw = 1;
        }
        func_00243D80(&D_00620300, TEX_VRAM(rec), tbw, TEX_PSM(rec), 0, 0, 1 << TEX_TW(rec), 1 << TEX_TH(rec));
        func_0025A960(0);
        if ((rec->u.tex & 0x3F00000000) == 0x1B00000000 || (rec->u.tex & 0x3F00000000) == 0x2C00000000 ||
            (rec->u.tex & 0x3F00000000) == 0x2400000000) {
            quads = 8;
        } else {
            quads = 1;
        }
        if (func_002212D8()) {
            func_0025A720(func_002212A0());
        }
        func_002433A8(0, 0);
        func_002440A8(&D_00620300, (char *)rec + quads * 16);
        func_002433A8(0, 0);
        if (func_002212D8()) {
            func_0025A700(func_002212A0());
        }
        if ((rec->u.tex & 0x3F00000000) == 0x1B00000000 || (rec->u.tex & 0x3F00000000) == 0x2C00000000 ||
            (rec->u.tex & 0x3F00000000) == 0x2400000000) {
            rec->env = D_00620300;
            id = TEX_ID(rec);
            entry = &D_00621310[id];
            entry->x4.index = TEX_VRAM(rec);
            entry->x4.tbw = tbw;
            entry->rec = rec;
            entry->x4.count = TEX_COUNT(rec);
            entry->x4.active = 1;
            texSize = (1 << TEX_TW(rec)) << TEX_TH(rec);
            if ((rec->u.tex & 0x3F00000000) != 0x1B00000000) {
                texSize >>= 1;
            }
            texBytes += texSize;
            func_002189F8("loading spackle tex id %d @ %08x (%p)\n", id, TEX_VRAM(rec), rec);
        } else {
            clutSize = (1 << TEX_TW(rec)) << TEX_TH(rec);
            if (clutSize == 16) {
                clutCount[0]++;
            } else {
                clutCount[1]++;
            }
            if (clutSize < 64) {
                clutSize = 64;
            }
            clutBytes += clutSize * 4;
            if (top <= TEX_VRAM(rec)) {
                D_0029E21C = D_0029E220 = clutSize + (TEX_VRAM(rec) << 6);
                top = TEX_VRAM(rec);
            }
            func_002189F8("loading clut %x %x, next %x ", TEX_VRAM(rec), TEX_VRAM(rec) << 6, D_0029E21C);
            func_002189F8("width %d, height %d, sizeD %d\n", 1 << TEX_TW(rec), 1 << TEX_TH(rec), TEX_LEVELS(rec));
        }
        prev = rec;
        rec = (TexRec *)((char *)rec + (rec->x0 >> 2) * 16);
    } while (prev->x0 != 0);
    func_002189F8("Resident Texture Loaded = %d bytes\n", texBytes);
    func_002189F8("CLUTs Loaded = %d bytes, 16 color cluts(%d), 256 color cluts(%d)\n", clutBytes, clutCount[0], clutCount[1]);
    func_002189F8("StartVramAddr = %x\n", D_0029E220);
    D_0029E220 = D_0029E21C = ((unsigned int)D_0029E220 >> 11) + 1 << 11;
}
