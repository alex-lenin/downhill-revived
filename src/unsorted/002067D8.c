/* UNIT 0x002067D8 0x00206878 */
typedef struct Slot {
    char pad0[4];
    unsigned int *x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    char pad1C[0x3C - 0x1C];
} Slot;

typedef struct Pool {
    char pad0[4];
    int x4;
    int x8;
    int xC;
    Slot *slots;
    char pad14[0x20 - 0x14];
    unsigned short *count;
} Pool;

extern int D_0029E160;
extern void func_00204A98(Slot *, Pool *);

void func_002067D8(Pool *pool)
{
    unsigned int best = 0x7FFFFFF;
    Slot *found = 0;
    int i;

    for (i = 0; i < *pool->count; i++) {
        if (pool->slots[i].x4) {
            unsigned int key = *pool->slots[i].x4 >> 18;
            if (key < best) {
                best = key;
                found = &pool->slots[i];
            }
        }
    }
    if (found) {
        found->x18 = 0;
        found->x14 = D_0029E160;
        found->x10 = pool->xC;
        found->xC = pool->x8;
        found->x8 = pool->x4;
        func_00204A98(found, pool);
    }
}
