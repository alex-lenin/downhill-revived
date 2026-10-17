/* UNIT 0x00163DA0 0x00163E98 */
struct Elem { char data[0x20]; };

struct Pool {
    struct Elem *elems;
    char pad4[0x6A];
    unsigned char random2;
};

struct List {
    char pad0[0x16];
    short count;
    short indices[1];
};

extern int func_00222BD8(int, int);
extern int func_00222B08(int, int);

struct Elem *func_00163DA0(struct Pool *pool, struct List *list, struct Elem *exclude)
{
    int idx;
    int n;

    if (pool->random2) {
        idx = func_00222BD8(0, list->count - 1);
    } else {
        idx = func_00222B08(0, list->count - 1);
    }
    if (exclude != 0) {
        for (n = 0; n < list->count; n++) {
            if (&pool->elems[list->indices[idx]] != exclude) {
                break;
            }
            idx++;
            if (idx >= list->count) {
                idx = 0;
            }
        }
    }
    return &pool->elems[list->indices[idx]];
}
