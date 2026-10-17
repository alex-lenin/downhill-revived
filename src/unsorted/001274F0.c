/* UNIT 0x001274F0 0x001275B0 */
typedef struct Pool {
    char pad0[0x1B580];
    unsigned char order[200];
    unsigned char x1B648;
    char pad1B649[0x7];
} Pool; /* 0x1B650 bytes */

typedef struct Manager {
    char head[0x5DD0];
    Pool pool;
    int x21420;
    int x21424;
    int x21428;
    int x2142C;
    int x21430;
    unsigned char x21434;
} Manager;

extern void *func_00254204(void *dst, int value, int size);

void func_001274F0(Manager *mgr)
{
    Pool *pool;
    int i;
    unsigned char *p;

    func_00254204(mgr, 0, sizeof(mgr->head));
    pool = &mgr->pool;
    func_00254204(pool, 0, sizeof(Pool));
    i = 199;
    mgr->x21420 = 0;
    mgr->x21424 = 0;
    mgr->x21428 = 0;
    mgr->x2142C = 1;
    p = &pool->order[199];
    do {
        *p-- = i;
    } while (--i >= 0);
    pool->x1B648 = 0;
    mgr->x21430 = 0;
    mgr->x21434 = 1;
}
