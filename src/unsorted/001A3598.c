/* UNIT 0x001A3598 0x001A38F8 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: loop.c hoists sp+0x9C (copy.items) into a callee-saved reg instead of stride 24, cascading reg choices;
   best diff 25/216 words */
typedef struct Rider {
    char pad0[0x6634];
    float x6634;
    char pad6638[0x7908 - 0x6638];
    int x7908;
    char pad790C[0x796C - 0x790C];
    int x796C;
    char pad7970[0x7978 - 0x7970];
    int x7978;
    char pad797C[0x798C - 0x797C];
    int x798C;
    char pad7990[0x7998 - 0x7990];
    int x7998;
    char pad799C[0x7A5C - 0x799C];
    int x7A5C;
} Rider;

typedef struct {
    Rider *rider;
    int x4;
    int x8;
    int xC;
    int x10;
} Entry14;

typedef struct {
    Entry14 entries[10];
    char padC8[0x138 - 0xC8];
    int x138;
} ObjS;

typedef struct {
    int x0[6];
} Item;

typedef struct {
    int x0;
    char pad4[0x14];
    int x18;
    char pad1C[0x5C - 0x1C];
    Item items[3];
    char padA4[0xD8 - 0xA4];
} TableEntry;

typedef struct {
    char pad0[0x2A0];
    int x2A0;
} GlobalB25;

typedef struct {
    char pad0[0x501A8];
    int x501A8;
} GameAF;

/* Single-field views of the current TableEntry (D_004225A0 + 0x7668 + x2A0 * 0xD8):
   Big7668 = x0, Big7680 = x18, Big76C4 = items[0].x0[0] (items at +0x5C, 0x18 apart).
   Written relative to D_004225A0 because that address form keeps the retail allocation. */
typedef struct {
    char pad0[0x76C4];
    int x76C4;
} Big76C4;

typedef struct {
    char pad0[0x7668];
    int x7668;
} Big7668;

typedef struct {
    char pad0[0x7680];
    int x7680;
} Big7680;

typedef struct { char pad0[0x7668]; TableEntry tables[1]; } World;
extern World D_004225A0;
extern GlobalB25 D_004B25A0;
extern GameAF D_002AF2C0;
extern int D_004C8508[];

extern void func_001A38F8(void *unused, int *values, int *order, int count);
extern void func_00195AA0(void *dst, void *src);
extern void func_00195520(void *obj, int index, void *item);
extern void func_001956C0(void *list, int index, int arg2, int arg3, int arg4, int player, int arg6);
extern void func_00195828(void *obj, int arg1, int arg2, int arg3, int player, int arg5);
extern void func_00195960(void *obj, int arg1, int arg2, int arg3, int player, int arg5);
extern float func_001301B8(void *obj);
extern int func_001FA168(void *a0, int a1, int a2);

static inline Item *GetItem(Item *items, int index) { return items + index; }

/* Rank three table items against the x4 of obj's entries 0..2 (func_001A38F8 sorts the six),
   write the best three back into the current table, then run the per-entry x0/x18 checks. */
void func_001A3598(ObjS *obj)
{
    int values[6];
    int order[6];
    TableEntry copy[1];
    int count;
    int i;
    int j;
    int *sorted;
    TableEntry *localTable;

    count = D_002AF2C0.x501A8;
    obj->x138 = 0;
    {
        int q;
        int tableStride;
        int *dest;
        int *source;
        char *p;
        i = 0;
        sorted = order;
        localTable = copy;
        tableStride = 0xD8;
        dest = values;
        source = (int *)((char *)obj - 0x38);
        q = 0;
        do {
            if (i < 3) {
                p = (char *)&D_004225A0 + D_004B25A0.x2A0 * tableStride;
                *dest = ((Big76C4 *)(p + q))->x76C4;
            } else {
                *dest = *source;
            }
            i++;
            dest++;
            source = (int *)((char *)source + 0x14);
            q += 0x18;
        } while (i < 6);
    }
    func_001A38F8(obj, values, sorted, 6);
    {
        int off;
        off = D_004B25A0.x2A0 * 0xD8 + 0x7668;
        func_00195AA0(localTable, (void *)((char *)&D_004225A0 + off));
    }
    for (j = 0; j < 3; j++) {
        int v;
        v = sorted[j];
        if (v < 3) {
            int off;
            off = D_004B25A0.x2A0 * 0xD8 + 0x7668;
            /* &copy[0].items[v] */
            func_00195520((void *)((char *)&D_004225A0 + off), j, (Item *)((int)copy + (0x5C + v * 24)));
        } else {
            Entry14 *e;
            Rider *r;
            int off;
            int player;
            e = &obj->entries[v - 3];
            r = e->rider;
            if (r != 0) {
                player = 0; /* x7A5C == 1: player = x7978 + 1 */
                if ((r->x7A5C ^ 1) == 0) player = r->x7978 + 1;
                off = D_004B25A0.x2A0 * 0xD8 + 0x7668;
                func_001956C0((void *)((char *)&D_004225A0 + off), j, e->x4, r->x796C, r->x7908, player, r->x7998);
                obj->x138 |= (1 << j);
            }
        }
    }
    if (count > 0) {
        Entry14 *e;
        int v;
        int tableStride;
        int tableAddress;
        tableStride = 0xD8;
        j = count;
        e = &obj->entries[0];
        do {
            tableAddress = D_004B25A0.x2A0 * tableStride;
            tableAddress += (int)&D_004225A0;
            if (((Big7668 *)tableAddress)->x7668 < e->xC) {
                Rider *r;
                int off;
                int player;
                r = e->rider;
                player = 0;
                if ((r->x7A5C ^ 1) == 0) player = r->x7978 + 1;
                off = D_004B25A0.x2A0 * tableStride + 0x7668;
                func_00195828((void *)((char *)&D_004225A0 + off), e->xC, r->x796C, r->x7908, player, r->x7998);
                obj->x138 |= 8;
            }
            {
                Rider *r;
                Rider *rBefore;
                int delta;
                int total;
                int scoreStride;
                scoreStride = 0xD8;
                rBefore = e->rider;
                v = ((Big7680 *)((char *)&D_004225A0 + D_004B25A0.x2A0 * scoreStride))->x7680;
                delta = func_001FA168(D_004C8508, rBefore->x798C, (rBefore->x796C >> 5) - 1);
                delta += (int)func_001301B8(e->rider);
                r = e->rider;
                delta += e->xC;
                total = delta + (int)r->x6634;
                if (v < total) {
                    int off;
                    int player;
                    player = 0;
                    if ((r->x7A5C ^ 1) == 0) player = r->x7978 + 1;
                    off = D_004B25A0.x2A0 * scoreStride + 0x7668;
                    func_00195960((void *)((char *)&D_004225A0 + off), total, r->x796C, r->x7908, player, r->x7998);
                    obj->x138 |= 0x10;
                }
            }
            j--;
            e++;
        } while (j != 0);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_001A3598);
#endif
