/* UNIT 0x001275B0 0x00127738 -fno-gcse */
typedef struct Item {
    char pad0[0xD0];
    char xD0[0x14];
    int xE4;
    char padE8[8];
} Item;

typedef struct Buffer {
    char pad0[0x1B580];
    unsigned char order[200];
    unsigned char count;
    char pad1B649[7];
} Buffer;

typedef struct Pool {
    char pad0[0x10];
    Item items[100];
    Buffer buffer;
    int x21420;
    int x21424;
    int x21428;
    int x2142C;
    int x21430;
    unsigned char x21434;
    int x21438[100];
    int x215C8[100][24];
    int x23B48[24];
} Pool;

extern int func_00254204(void *, int, int);
extern void func_001BEE08(void *, int);

void func_001275B0(Pool *pool)
{
    Buffer *buffer = &pool->buffer;
    Item *item;
    unsigned char *order;
    int *out, *row;
    int i, j, n;

    func_00254204(buffer, 0, sizeof(Buffer));
    pool->x21420 = 0;
    pool->x21424 = 0;
    pool->x21428 = 0;
    pool->x2142C = 1;
    order = &buffer->order[199];
    for (n = 199; n >= 0; n--)
        *order-- = n;
    buffer->count = 0;
    pool->x21430 = 0;
    pool->x21434 = 1;
    out = pool->x21438;
    for (i = 0; i < 100; i++) {
        *out = 0;
        for (j = 23; j >= 0; j--)
            pool->x215C8[i][j] = 0;
        item = &pool->items[i];
        func_001BEE08(item->xD0, 2);
        item->xE4 = 1;
        out++;
    }
    for (i = 23; i >= 0; i--)
        pool->x23B48[i] = 0;
}
