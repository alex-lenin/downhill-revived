/* UNIT 0x0023BCB8 0x0023BE10 */
typedef struct Entry {
    long x0;
    long x8;
    int offset; /* x10 */
    int size;   /* x14 */
} Entry;

typedef struct Ring {
    char pad0[0x8];
    int x8;
    char padC[0x44];
    Entry *entries; /* x50 */
    int count;      /* x54 */
    int used;       /* x58 */
    int x5C;
} Ring;

/* Distance from `to` forward to `from` in a circular buffer of `size` bytes.
   Inlined: integrate.c folds the divide-by-zero check's 0 into $zero. */
static inline int RingDistance(int from, int to, int size)
{
    return (from + size - to) % size;
}

int func_0023BCB8(Ring *ring, Entry *input)
{
    int index;
    int active;
    int size;
    int offset;
    int amount;
    int used;
    int remaining;
    Entry *entry;

    active = 1;
    index = (ring->x5C - ring->used + ring->count) % ring->count;
    size = ring->x8 << 11;
    if (ring->used > 0) {
        while (active && (entry = &ring->entries[index])->size != 0 && input->size != 0) {
            offset = entry->offset;
            if (RingDistance(offset, input->offset, size) < input->size) {
                amount = input->offset + input->size - offset;
                if (amount >= entry->size)
                    amount = entry->size;
                remaining = entry->size - amount;
                offset = (offset + amount) % size;
                entry->size = remaining;
                entry->offset = offset;
                if (remaining == 0) {
                    if (entry->x0 >= 0) {
                        entry->x0 = -1;
                        entry->x8 = -1;
                        entry->offset = 0;
                        entry->size = 0;
                    }
                    used = ring->used - 1;
                    ring->used = used > remaining ? used : 0;
                }
            } else {
                active = 0;
            }
            index = (index + 1) % ring->count;
        }
    }
    return 0;
}
