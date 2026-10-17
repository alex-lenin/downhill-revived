/* UNIT 0x00208A20 0x00208AB0 */
struct Slot {
    int handle;
};

struct Pool {
    struct Slot *slots[30];
    int count;
};

extern void func_00201918(int);
extern void func_002018E8(void);
extern int func_002013F8(void);

void func_00208A20(struct Pool *pool)
{
    int i;

    for (i = 0; i < pool->count; i++) {
        func_00201918(pool->slots[i]->handle);
        pool->slots[i]->handle = 0;
        pool->slots[i] = 0;
    }
    pool->count = 0;
    func_002018E8();
    while (func_002013F8()) {}
}
