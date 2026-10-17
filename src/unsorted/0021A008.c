/* UNIT 0x0021A008 0x0021A4E8 .rodata=0x2987A8:0x298940 */
typedef struct Bin {
    unsigned short count;
    unsigned char w, h;
    unsigned int size;
} Bin;

typedef struct TexList {
    unsigned int count;
    int x4;
    Bin tex[1];
} TexList;

#define MAX_FIT(a, b) ((a) > (b) ? (b) : (a))

extern char D_004D1A18[];
extern int D_0029E220, D_0029E22C;
extern TexList *func_0020D170(void *, int);
extern int func_0021A4E8(Bin *, unsigned char, unsigned char, unsigned short *);
extern unsigned int func_0021A530(Bin *, unsigned int, unsigned short *);
extern void func_002189F8(const char *, ...);
extern unsigned int func_001FD238(float);

int func_0021A008(Bin *out) {
    Bin bins[15];
    unsigned short index;
    unsigned int total;
    int i;
    unsigned int j, nbins;
    unsigned int avail, used, best;
    float ratio;
    TexList *list;

    total = 0;
    nbins = 0;
    used = 0;
    for (i = 0; i < 15; i++) {
        out[i].size = 0;
        out[i].count = 0;
        bins[i].size = 0;
        bins[i].count = 0;
    }
    list = func_0020D170(D_004D1A18, 0);
    for (j = 0; j < list->count; j++) {
        if (func_0021A4E8(bins, list->tex[j].w, list->tex[j].h, &index)) {
            bins[index].count += list->tex[j].count;
        } else {
            bins[nbins].size = list->tex[j].size;
            out[nbins].size = list->tex[j].size;
            bins[nbins].count = list->tex[j].count;
            nbins++;
        }
    }
    func_002189F8("Allocated %i bins.\n", nbins);
    for (i = 0; i < nbins; i++) {
        total += bins[i].count * bins[i].size;
        func_002189F8("Bin %i has %i textures of %i bytes each.\n", i, bins[i].count, bins[i].size);
    }
    avail = (D_0029E22C - D_0029E220) * 4;
    avail -= 0x30000;
    func_002189F8("totalAvailSpace after CLUTs = %d\n", avail);
    for (i = 0; i < nbins; i++) {
        if (bins[i].count == 0) bins[i].count = 1;
        avail -= out[i].size;
    }
    ratio = (float)avail / (float)total;
    if (1.0f < ratio) ratio = 1.0f;
    func_002189F8("We can put %i%% (%i bytes) of our texture (%i bytes) in memory at a time.\n",
                  (int)(ratio * 100.0f), avail, total);
    for (i = 0; i < nbins; i++) {
        func_002189F8("Bin %i adjusted from %i blocks ", i, bins[i].count);
        if (func_001FD238(bins[i].count * ratio) + 1 <= bins[i].count) {
            out[i].count = func_001FD238(bins[i].count * ratio) + 1;
            used += (out[i].count - 1) * out[i].size;
        } else {
            out[i].count = bins[i].count;
            used += (out[i].count - 1) * out[i].size;
        }
        func_002189F8("down to %i blocks.  -  Total bin space = %i of %i\n", out[i].count, used, avail);
    }
    avail += 0x10000;
    func_002189F8("We have %i bytes left over in VRAM that we could fill.\n", avail - used);
    best = func_0021A530(out, 0, &index);
    for (i = 0; i < 15; i++) {
        while (best < avail - used && best != 0 && bins[index].count > out[index].count) {
            out[index].count++;
            used += out[index].size;
            func_002189F8("Added a %i bin. Now there are %i bytes avail. Largest tex is %i.\n",
                          out[index].size, avail - used, best);
        }
        best = func_0021A530(out, best, &index);
        if (best == 0) break;
    }
    return 0;
}
