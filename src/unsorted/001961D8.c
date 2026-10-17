/* UNIT 0x001961D8 0x001962D0 */
struct Owner {
    void *x0;
};

struct Slot {
    int x0;
    int x4;
    struct Owner *x8;
    int xC;
    int x10;
};

struct Pool {
    int types[6];
    struct Slot slots[150];
    int counts[6];
    char padding[0x18];
    int count;
    int n;
};

extern void func_001964A8(struct Slot *, unsigned int, float *, int);

static inline int slot_free(struct Slot *slot) {
    return slot->x8->x0 == 0;
}

int func_001961D8(struct Pool *pool, unsigned int type, float *pos) {
    int i;
    int index;

    if (type >= 6)
        return -1;
    for (i = 0; i < pool->count; i++) {
        if (slot_free(&pool->slots[i])) {
            func_001964A8(&pool->slots[i], type, pos, pool->types[type]);
            pool->counts[type]++;
            return pool->count - 1;
        }
    }
    if (pool->count < 150) {
        func_001964A8(&pool->slots[pool->count], type, pos, pool->types[type]);
        pool->counts[type]++;
        index = pool->count;
        pool->count = index + 1;
        return index;
    }
    return -1;
}
