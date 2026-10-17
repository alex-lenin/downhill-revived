/* UNIT 0x001ACA78 0x001ACAE0 */
struct Item {
    char pad0[0x64];
};

struct ItemPool {
    struct Item items[32];
    unsigned char freeList[32];
    unsigned char freeCount;
};

extern struct ItemPool D_004A9160;
extern int func_001ACA08(struct Item *item);

int func_001ACA78(struct Item *item)
{
    if (func_001ACA08(item)) {
        return 1;
    }
    D_004A9160.freeList[--D_004A9160.freeCount] = item - D_004A9160.items;
    return 0;
}
