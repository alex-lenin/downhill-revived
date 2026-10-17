/* UNIT 0x00233118 0x002331F0 */
typedef struct Pool {
    void *x0;
    void *x4[32];
    int x84[32];
    int x104[32];
    int x184;
    int x188;
    char pad18C[0x60];
    int x1EC;
    int x1F0;
    int x1F4;
} Pool;

extern void *func_001FD430(int size);
extern void *func_001FFC08(int size);
extern void *func_00235150(void *mem, int arg);

int func_00233118(Pool *pool, int count, int size, int arg) {
    int i;

    for (i = 0; i < count; i++) {
        pool->x84[i] = 0;
        pool->x4[i] = func_001FD430(size * 4);
        if (pool->x4[i] == 0) {
            return 1;
        }
        pool->x104[i] = -1;
    }
    pool->x184 = size;
    pool->x1EC = 1;
    pool->x188 = count;
    pool->x1F0 = 0;
    pool->x1F4 = 0;
    pool->x0 = func_00235150(func_001FFC08(8), arg);
    if (pool->x0 == 0) {
        return 1;
    }
    return 0;
}
