/* UNIT 0x0020AC80 0x0020B6B0 .sdata=0x29D9D4:0x29D9D8 */
#include "include_asm.h"

int D_0029D9D4 = 0xa00000;

/* GS TEX0 register: TBP0 = bits 0-13, PSM = bits 20-25, CBP = bits 37-50 */
#define TEX0_TBP0_MASK 0x3FFFUL
#define TEX0_PSM_MASK 0x3F00000UL
#define TEX0_CBP_MASK (0x3FFFUL << 37)

typedef struct Arena {
    int x0;
    int cursor;
    int previous;
} Arena;

struct Slot {
    char pad0[0x40];
    int x40;
    char pad44[0x4];
};

struct MapEntry {
    unsigned short key;
    unsigned short value;
};

struct Map {
    struct MapEntry entries[1000];
    int xFA0;
    int count;
    int last;
};

extern Arena D_00663B08;
extern struct Slot D_004D0380[];
extern char D_004D1A18[];
extern char D_004D30B0[];
extern char D_0029D9D8[];
extern struct Map D_00620360;
extern int *func_0020D170(void *, int);
extern int func_0020D1C8(void *, int);
extern int func_0020D200(void *, int);
extern int func_0020D238(void *, int);
extern int func_0020D8F8(void *, int, int, char *, int);

static __inline__ void Arena_Push(Arena *arena)
{
    int old = arena->previous;
    int p = (arena->cursor + 3) & ~3;

    arena->previous = p;
    arena->cursor = p + 4;
    *(int *)p = old;
}

static __inline__ void Arena_Pop(Arena *arena)
{
    arena->cursor = arena->previous;
    arena->previous = *(int *)arena->previous;
}

static __inline__ struct MapEntry *Map_Find(struct Map *map, unsigned short key)
{
    int mid, lo, hi;

    if (map->count == 0)
        return 0;
    mid = map->count >> 1;
    if (map->entries[mid].key < key) {
        lo = mid + 1;
        hi = map->count - 1;
    } else if (key < map->entries[mid].key) {
        lo = 0;
        hi = mid - 1;
    } else {
        return &map->entries[mid];
    }
    lo = lo < 0 ? 0 : lo;
    hi = hi < 0 ? 0 : hi;
    lo = lo > map->count - 1 ? map->count - 1 : lo;
    hi = hi > map->count - 1 ? map->count - 1 : hi;
    while (hi - lo >= 2) {
        if (key < map->entries[mid].key)
            hi = mid;
        else
            lo = mid;
        mid = (lo + hi) >> 1;
    }
    map->last = lo;
    if (key == map->entries[lo].key)
        return &map->entries[lo];
    if (key == map->entries[hi].key)
        return &map->entries[hi];
    return 0;
}

void func_0020AC80(int slot, int arg1)
{
    unsigned int *data;
    unsigned int *p;
    int *base;
    int delta;
    unsigned int i;
    unsigned int end;

    if (slot == 0)
        return;
    if (D_004D0380[slot].x40 != slot)
        return;

    Arena_Push(&D_00663B08);
    data = (unsigned int *)((D_00663B08.cursor + 15) & ~15);
    D_00663B08.cursor = (int)data + 40000;
    p = data + 1;

    delta = (int)func_0020D170(D_004D0380, slot) - D_0029D9D4;
    func_0020D8F8(data, arg1, func_0020D238(D_004D0380, 0), D_0029D9D8, 1);

    base = func_0020D170(D_004D0380, slot);
    {
        unsigned int n = base[0];
        unsigned int j;
        for (j = 0; j < n; j++)
            *++base += delta;
    }

    base = func_0020D170(D_004D0380, slot);
    i = 1;
    end = data[0];
    for (; i <= end; i++)
        *(int *)(*p++ + (int)base) += delta;

    end++;
    end += data[i];
    i++;
    p++;
    for (; i <= end; i++, p++) {
        if (*p != 0) {
            unsigned short *id = (unsigned short *)(*p + (int)base);
            struct MapEntry *found;

            if (*id != 0)
                *id += func_0020D1C8(D_004D1A18, slot);
            found = Map_Find(&D_00620360, *id);
            if (found != 0)
                *id = found->value;
        }
    }

    end++;
    end += data[i];
    i++;
    p++;
    for (; i <= end; i++, p++) {
        if (*p != 0) {
            unsigned long *tex = (unsigned long *)(*p + (int)base);
            unsigned long psm;

            if (tex != 0)
                *tex = (*tex & ~TEX0_CBP_MASK) | (((((*tex >> 37) & 0x3FFF) + (unsigned short)func_0020D200(D_004D1A18, slot)) & 0x3FFF) << 37);
            psm = *tex & TEX0_PSM_MASK;
            if (psm == 0x1B00000 || psm == 0x2C00000 || psm == 0x2400000)
                *tex = (*tex & ~TEX0_TBP0_MASK) | (((*tex & 0x3FFF) + (unsigned short)func_0020D200(D_004D30B0, slot)) & 0x3FFF);
        }
    }

    Arena_Pop(&D_00663B08);
}

static __inline__ void B088_Push(Arena *arena)
{
    int p = (arena->cursor + 3) & ~3;
    int old = arena->previous;

    arena->previous = p;
    arena->cursor = p + 4;
    *(int *)p = old;
}

static __inline__ int B088_Alloc(Arena *arena, int size)
{
    int p = (arena->cursor + 15) & ~15;

    arena->cursor = p + size;
    return p;
}

static __inline__ int B088_Begin(int amount)
{
    return amount;
}

void func_0020B088(int offset, int arg1, int arg2)
{
    int p, old, buffer, base, count, shift, amount;
    int *list;
    void *data;
    unsigned long *entry, word;

    if (!D_004D0380[0].x40) {
        B088_Push(&D_00663B08);
        buffer = B088_Alloc(&D_00663B08, 0x61a80);
        base = (int)func_0020D170(D_004D0380, 0);
        func_0020D8F8((void *)buffer, arg1, func_0020D238(D_004D0380, 0), D_0029D9D8, arg2);
        list = (int *)buffer;
        list = (int *)((char *)list + (*list * 4 + 4));
        list = (int *)((char *)list + (*list * 4 + 4));
        amount = *list++;
        if (amount > 0) {
            count = B088_Begin(amount);
            shift = offset / 64;
            do {
                p = *list;
                if (p) {
                    entry = (unsigned long *)(p + base);
                    word = *entry;
                    *entry = (word & ~((unsigned long)0x3fff << 37)) | (((((word >> 37) & 0x3fff) + shift) & 0x3fff) << 37);
                }
                --count;
                ++list;
            } while (count);
        }
        D_00663B08.cursor = D_00663B08.previous;
        D_00663B08.previous = *(int *)D_00663B08.previous;
    }
}

void func_0020B228(int *data, int arg1, int slot)
{
    int *base;
    int delta;
    int n0, n1, n2;
    int *list0;
    int *list1;
    int *list2;
    int *p;
    int i;

    base = func_0020D170(D_004D0380, slot);
    delta = (int)base - D_0029D9D4;
    if (D_004D0380[slot].x40 != slot)
        return;
    n0 = data[0];
    list0 = data + 1;
    p = list0 + n0;
    n1 = p[0];
    list1 = p + 1;
    p = list1 + n1;
    n2 = p[0];
    list2 = p + 1;
    if (slot == 0) {
        for (i = 0; i < n2; i++) {
            unsigned long *tex = (unsigned long *)(list2[i] + (int)base);
            if (tex != 0)
                *tex = (*tex & ~TEX0_CBP_MASK) | (((((*tex >> 37) & 0x3FFF) + arg1 / 64) & 0x3FFF) << 37);
        }
        return;
    }
    {
        unsigned int n = base[0];
        unsigned int j;
        for (j = 0; j < n; j++)
            base[j + 1] += delta;
    }
    for (i = 0; i < n0; i++)
        *(int *)(list0[i] + (int)base) += delta;
    for (i = 0; i < n1; i++) {
        if (list1[i] != 0) {
            unsigned short *id = (unsigned short *)(list1[i] + (int)base);
            struct MapEntry *found;

            if (*id != 0)
                *id += func_0020D1C8(D_004D1A18, slot);
            found = Map_Find(&D_00620360, *id);
            if (found != 0)
                *id = found->value;
        }
    }
    for (i = 0; i < n2; i++) {
        unsigned long *tex = (unsigned long *)(list2[i] + (int)base);
        if (tex != 0) {
            unsigned long psm;

            *tex = (*tex & ~TEX0_CBP_MASK) | (((((*tex >> 37) & 0x3FFF) + (unsigned short)func_0020D200(D_004D1A18, slot)) & 0x3FFF) << 37);
            psm = *tex & TEX0_PSM_MASK;
            if (psm == 0x1B00000 || psm == 0x2C00000 || psm == 0x2400000)
                *tex = (*tex & ~TEX0_TBP0_MASK) | (((*tex & 0x3FFF) + (unsigned short)func_0020D200(D_004D30B0, slot)) & 0x3FFF);
        }
    }
}

struct Entry {
    char pad0[0x58];
    int x58;
    char pad5C[0x4];
};

extern int D_0029E170;
extern struct Entry D_004D5DE0[];

int func_0020B600(void)
{
    struct Entry *e = &D_004D5DE0[D_0029E170];
    return e->x58;
}

extern char D_004D0350[],D_004D0368[],D_004D4748[];
extern void func_00223A98(void*,int,int);
extern void func_0020D060(void*);
void func_0020B628(int a,int b) {
    if(b==65535 && a) {
        func_00223A98(D_004D0350,0x93caf0,0xc3500);
        func_00223A98(D_004D0368,D_0029D9D4-16,0x1600000);
        func_0020D060(D_004D0380);
        func_0020D060(D_004D1A18);
        func_0020D060(D_004D30B0);
        func_0020D060(D_004D4748);
    }
}
