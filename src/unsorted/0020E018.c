/* UNIT 0x0020E018 0x0020E480 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: different register allocation (retail spills queue/name pointers to the stack), so offsets and order differ;
   225 aligned rows differ (built 0x408 vs 0x468; prior best 165/282) */
/* ISO9660 sector buffer (2048 bytes + slack) and the flat directory tree built from it */
extern unsigned char D_004F2980[];
extern unsigned char D_004DA280[];
extern char D_0029E180;
extern char D_0029E181;
extern char D_0029E182;
extern char D_00298158[];
extern char D_002981B0[];
extern int func_0020E480(int offset);               /* little-endian u32 at D_004F2980 + offset */
extern int func_0020E4D0(int count, int sector, void *dst); /* read sectors */
extern void *func_0020E7B0(int sector);              /* find the directory entry for a sector */
extern void func_0020D028(char *fmt, ...);
extern void func_00254D08(char *fmt, ...);

/* one tree entry, 0x24 bytes; a directory is followed by sectors * 0xB4 bytes of child slots */
typedef struct IsoEntry {
    int extent;              /* x0: first sector */
    unsigned short sectors;  /* x4: size in sectors */
    unsigned short x6;
    int size;                /* x8: size in bytes */
    unsigned char flags;     /* xC: ISO file flags, 2 = directory */
    char name[23];           /* xD */
} IsoEntry;

/* a directory entry seen as its child list */
typedef struct IsoDir {
    char pad0[6];
    unsigned short count;    /* x6 */
    char pad8[0x18];
    IsoEntry *items[45];     /* x20 */
} IsoDir;

/* directories still to scan */
typedef struct DirQueue {
    long sector;
    long sectors;
} DirQueue;

/* ISO9660 directory record fields */
#define REC_LEN(o)      D_004F2980[(o)]
#define REC_FLAGS(o)    D_004F2980[(o) + 0x19]
#define REC_NAMELEN(o)  D_004F2980[(o) + 0x20]
#define REC_NAME(o)     (&D_004F2980[(o) + 0x21])

/* Read the disc's directory tree into D_004DA280, breadth first from the root record of the
   primary volume descriptor (sector 0x10). */
void func_0020E018(void)
{
    DirQueue queue[256];
    int total;      /* queued directories */
    int queued;     /* directories taken from the queue */
    int need;       /* set by each stored entry, cleared when a directory is queued */
    int sector;
    int sectors;
    int s;
    int i;
    int off;
    int slot;
    int keep;
    IsoEntry *cur;
    IsoDir *parent;

    D_0029E180 = 0x14;
    cur = (IsoEntry *)D_004DA280;
    D_0029E181 = 1;
    D_0029E182 = 0;
    total = 0;
    queued = 0;
    need = 0;
    for (i = 0x80F; i >= 0; i--)
        D_004F2980[i] = 0;
    for (i = 0x186A0; i >= 0; i--)
        D_004DA280[i] = 0;

    /* the root directory, from the primary volume descriptor */
    total = 1;
    func_0020E4D0(1, 0x10, D_004F2980);
    sector = func_0020E480(0x9C + 2);
    sectors = (func_0020E480(0x9C + 10) + 0x7FF) / 2048;
    queue[0].sector = sector;
    queue[0].sectors = sectors;

    while (queued != total) {
        parent = (IsoDir *)cur;
        slot = 0;
        sector = queue[queued].sector;
        sectors = queue[queued].sectors;
        queued++;
        for (s = 0; s < sectors; s++) {
            func_0020E4D0(1, sector + s, D_004F2980);
            if (REC_LEN(0) == 0)
                continue;
            off = 0;
            do {
                long extent = func_0020E480(off + 2);
                long size = func_0020E480(off + 10);

                if (REC_NAMELEN(off) == 1 && REC_NAME(off)[0] == 1) {
                    keep = 0;                           /* ".." */
                } else if (REC_NAMELEN(off) == 1 && REC_NAME(off)[0] == 0) {
                    /* "." */
                    keep = 0;
                    if (queued >= 2) {
                        /* sub-directory: its children go to its own entry */
                        parent = func_0020E7B0(sector);
                        if (parent == 0)
                            func_0020D028(D_00298158, D_004DA280);
                    } else {
                        /* root: keep "." as the tree's first entry */
                        keep = 1;
                        cur->name[0] = '.';
                        cur->name[1] = 0;
                    }
                } else {
                    keep = 1;
                    need = 1;
                    parent->items[slot] = cur;
                    slot++;
                    parent->count++;
                }

                if (keep) {
                    unsigned char flags = REC_FLAGS(off);
                    int len;

                    cur->extent = extent;
                    cur->sectors = ((unsigned long)(size - 1) >> 11) + 1;
                    cur->size = size;
                    cur->flags = flags;
                    if (flags == 2 && need) {
                        queue[total].sector = extent;
                        queue[total].sectors = (unsigned long)(size + 0x7FF) >> 11;
                        need = 0;
                        total++;
                    }
                    len = REC_NAMELEN(off);
                    for (i = 0; i < len; i++)
                        cur->name[i] = REC_NAME(off)[i];
                    cur->name[i] = 0;
                    if (cur->flags == 2) {
                        /* leave room for the directory's child slots */
                        unsigned int n = cur->sectors;
                        unsigned int k;

                        for (k = 0; k < n; k++)
                            cur = (IsoEntry *)((char *)cur + 0xB4);
                    }
                    cur++;
                }
                off += REC_LEN(off);
            } while (REC_LEN(off) != 0);
        }
    }
    func_00254D08(D_002981B0, (char *)cur - (char *)D_004DA280);
}
#undef REC_LEN
#undef REC_FLAGS
#undef REC_NAMELEN
#undef REC_NAME
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0020E018);
#endif
