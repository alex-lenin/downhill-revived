/* UNIT 0x0021B668 0x0021B798 */
typedef struct EntryBits {
    unsigned int index : 14;
    unsigned int pad : 6;
    unsigned int bank : 5;
    unsigned int count : 3;
    unsigned int pad2 : 3;
    unsigned int active : 1;
} EntryBits;

typedef struct Entry {
    int x0;
    union {
        EntryBits b;
        unsigned int word;
    } x4;
    int x8;
    int xC;
} Entry;

typedef struct Ref {
    unsigned short index : 14;
    unsigned short flags : 2;
} Ref;

typedef struct Slot {
    int x0;
    int x4;
} Slot;

typedef struct Ctx Ctx;

extern int D_0029E218;
extern int D_0029E224;
extern Entry D_00621310[];
extern Slot D_00625190[][300];

extern void func_00218A70(unsigned int, Ref *, Ctx *, long *);
extern void func_0021B328(int, long *, Ctx *, int, Ref *);

void func_0021B668(unsigned int id, Ref *ref, Ctx *ctx, long *out)
{
    Entry *entry;
    long psm;
    int stamp;

    if (id != 0) {
        psm = *(long *)ref & 0x3F00000;
        if (psm != 0x1B00000 && psm != 0x2C00000 && psm != 0x2400000 && id != 999) {
            stamp = ++D_0029E218;
            entry = &D_00621310[id];
            if (entry->x4.b.active) {
                D_00625190[entry->x4.b.bank][entry->x8].x0 = stamp;
                ref->index = 0;
                ref->index |= entry->x4.b.index;
                if (entry->x4.word & 0x0E000000 /* count != 0 */) {
                    func_0021B328(id, out, ctx, entry->x4.b.count, ref);
                } else {
                    *out = 0;
                }
            } else {
                func_00218A70(id, ref, ctx, out);
            }
        }
    } else {
        *(long *)ref = (unsigned int)(D_0029E224 | 0x24120000) | (0x8000L << 18);
    }
}
