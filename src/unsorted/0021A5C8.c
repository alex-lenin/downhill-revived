/* UNIT 0x0021A5C8 0x0021AC08 .sbss=0x29E208:0x29E210 */
typedef struct {
    char pad[8];
    unsigned short x8;
    unsigned char xA, xB;
    int xC;
} Pent;

typedef struct {
    unsigned short x8;
    unsigned char xA, xB;
    int xC;
} PEntry;

typedef struct {
    unsigned int count;
    int x4;
    PEntry entries[1];
} TList;

typedef struct {
    unsigned int x0;
    int x4;
    union {
        unsigned long tex;
        struct {
            unsigned long x8 : 16;
            unsigned long pad : 43;
            unsigned long x3B : 5;
        } b;
    } u;
} TexRec;

typedef struct {
    char pad[0x44];
    unsigned char x44;
    char pad45[3];
} TexNode;

typedef struct {
    char pad[0x168C];
    int x168C;
} Bank;

typedef struct {
    unsigned int key;
    unsigned short val;
    short pad;
} SEntry;

typedef struct {
    SEntry e[1000];
    int max;
    int count;
    int last;
} SMap;

typedef struct {
    unsigned short key;
    unsigned short val;
} GEntry;

typedef struct {
    GEntry e[1000];
    int xFA0;
    int count;
    int last;
} GMap;

extern char D_004D1A18[];
extern char D_00298940[];
TList *D_0029E208 __attribute__((section(".sbss"))) = 0;
TexRec *D_0029E20C __attribute__((section(".sbss"))) = 0;
extern GMap D_00620360;
extern TList *func_0020D170(void *, int);
extern int func_0020D190(void *, void *);
extern int func_0021A578(unsigned char, unsigned char, TList *);
extern int func_002189F8(char *, ...);
extern void func_0021AC08(void *);
extern void func_0020D1E8(void *, int, int);
extern int func_0020D1C8(void *, int);

static __inline__ void SInit(SMap *map) {
    map->max = 1000;
    map->count = 0;
    map->last = 0;
}

static __inline__ SEntry *SFind(SMap *map, unsigned int key) {
    int mid, lo, hi;
    if (map->count == 0) return 0;
    mid = map->count >> 1;
    if (map->e[mid].key < key) {
        lo = mid + 1; hi = map->count - 1;
    } else if (key < map->e[mid].key) {
        lo = 0; hi = mid - 1;
    } else return &map->e[mid];
    lo = lo < 0 ? 0 : lo;
    hi = hi < 0 ? 0 : hi;
    lo = lo > map->count - 1 ? map->count - 1 : lo;
    hi = hi > map->count - 1 ? map->count - 1 : hi;
    while (hi - lo >= 2) {
        if (key < map->e[mid].key) hi = mid;
        else lo = mid;
        mid = (lo + hi) >> 1;
    }
    map->last = lo;
    return key == map->e[lo].key ? &map->e[lo] :
        (key == map->e[hi].key ? &map->e[hi] : 0);
}
static __inline__ SEntry *SInsert(SMap *map, unsigned int key, unsigned short value, unsigned char *flag) {
    SEntry *found = SFind(map, key);
    SEntry *pos, *dst, *d;
    if (found == 0) {
        pos = &map->e[map->last];
        while (pos->key < key && pos < &map->e[map->count]) pos++;
        dst = &map->e[map->count];
        d = dst;
        while (dst != pos) {
            dst--;
            d->key = (d-1)->key;
            d->val = (d-1)->val;
            d--;
        }
        found = pos;
        found->key = key;
        found->val = value;
        map->count++;
        *flag = 1;
    } else { *flag = 0; }
    return found;
}
static __inline__ GEntry *GFind(GMap *map, unsigned short key) {
    int mid, lo, hi;
    if (map->count == 0) return 0;
    mid = map->count >> 1;
    if (map->e[mid].key < key) {
        lo = mid + 1; hi = map->count - 1;
    } else if (key < map->e[mid].key) {
        lo = 0; hi = mid - 1;
    } else return &map->e[mid];
    lo = lo < 0 ? 0 : lo;
    hi = hi < 0 ? 0 : hi;
    lo = lo > map->count - 1 ? map->count - 1 : lo;
    hi = hi > map->count - 1 ? map->count - 1 : hi;
    while (hi - lo >= 2) {
        if (key < map->e[mid].key) hi = mid;
        else lo = mid;
        mid = (lo + hi) >> 1;
    }
    map->last = lo;
    if (key == map->e[lo].key) return &map->e[lo];
    if (key == map->e[hi].key) return &map->e[hi];
    return 0;
}
static __inline__ GEntry *GInsert(GMap *map, unsigned short key, unsigned short value) {
    GEntry *found = GFind(map, key);
    GEntry *pos, *dst, *d;
    if (found) {  return found; }
    pos = &map->e[map->last];
    while (pos->key < key && pos < &map->e[map->count]) pos++;
    dst = &map->e[map->count];
    d = dst;
    while (dst != pos) {
        dst--;
        d->key = (d-1)->key;
        d->val = (d-1)->val;
        d--;
    }
    found = pos;
    found->key = key;
    found->val = value;
    map->count++;
    
    return found;
}

int func_0021A5C8(TList *tex) {
    unsigned int tmp[16];
    SMap map;
    unsigned char flag;
    TexRec *rec;
    TexRec *prev;
    TexNode *node;
    int idx;
    int base;
    unsigned int best;
    int i;
    int r;
    Pent *e;
    unsigned short old;
    SEntry *found;

    D_0029E208 = func_0020D170(D_004D1A18, 0);
    rec = (TexRec *)tex + tex->x4;
    idx = func_0020D190(D_004D1A18, tex);
    node = (TexNode *)((char *)D_004D1A18 + idx * 0x48);
    if (node->x44 != 0)
        return 0;
    node->x44 = 1;
    {
        int k;
        for (k = 0; k < 15; ++k)
            tmp[k] = 16;
    }
    SInit(&map);
    func_0021AC08(tex);
    if (tex != D_0029E208) {
        for (i = 0; i < tex->count; i++) {
            r = func_0021A578(tex->entries[i].xA, tex->entries[i].xB, D_0029E208);
            if (r == -1) {
                tmp[i] = D_0029E208->count;
                D_0029E208->entries[D_0029E208->count].xC = tex->entries[i].xC;
                D_0029E208->entries[D_0029E208->count].x8 = tex->entries[i].x8;
                D_0029E208->count++;
            } else {
                tmp[i] = r;
                e = (Pent *)((char *)D_0029E208 + r * 8);
                old = e->x8;
                e->x8 += tex->entries[i].x8;
                /* The (unused) result must be assigned: without it gcse leaves a
                   USE after the call, which changes register allocation. */
                r = func_002189F8(D_00298940, r, old, old, e->x8, e->x8);
            }
        }
    }
    if (idx == 0) {
        map.count = 0;
        map.last = 0;
        D_00620360.count = 0;
        D_00620360.last = 0;
        func_0020D1E8(D_004D1A18, 0, 0);
    } else {
        D_0029E20C->x0 = (rec - D_0029E20C) * 4;
        func_0020D1E8(D_004D1A18, idx, ((Bank *)D_004D1A18)->x168C);
    }
    base = func_0020D1C8(D_004D1A18, idx);
    best = func_0020D1C8(D_004D1A18, idx);
    do {
        rec->u.b.x8 += base;
        if (best < rec->u.b.x8)
            best = rec->u.b.x8;
        if ((rec->u.tex & 0x7000000000000L) == 0) {
            found = SInsert(&map, rec->x4, rec->u.b.x8, &flag);
            if (idx == 0) goto adv;
            if (flag != 0) goto upd;
            GInsert(&D_00620360, rec->u.b.x8, found->val);
            rec->u.b.x8 = found->val;
        }
upd:
        if (idx != 0) {
            rec->u.b.x3B = tmp[rec->u.b.x3B];
        }
adv:
        prev = rec;
        rec = prev + (prev->x0 >> 2);
    } while (prev->x0 != 0);
    /* The terminal record has zero size, so rec stays on it. */
    ((Bank *)D_004D1A18)->x168C = best;
    D_0029E20C = rec;
    return 0;
}
