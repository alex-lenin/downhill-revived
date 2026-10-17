/* UNIT 0x00208B40 0x00208BF8 */
typedef struct Pool {
    unsigned int *slots[20];
    int count;
} Pool;

extern void func_00201C58(unsigned int);

void func_00208B40(Pool *pool) {
    int i;

    if (pool->count > 0) {
        for (i = 0; i < 20; i++) {
            if (pool->slots[i] && *pool->slots[i] != 0xFFFFFFFF && *pool->slots[i] != 0) {
                func_00201C58(*pool->slots[i]);
                *pool->slots[i] = 0;
                pool->count--;
                pool->slots[i] = 0;
            } else if (pool->slots[i] && *pool->slots[i] == 0) {
                pool->count--;
                pool->slots[i] = 0;
            }
        }
    }
}
