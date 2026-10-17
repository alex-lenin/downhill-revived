/* UNIT 0x00138998 0x001389B8 */
typedef struct Item {
    char pad0[0x8];
    int flags;
} Item;

typedef struct Mgr {
    char pad0[0x31714];
    Item *x31714;
} Mgr;

void func_00138998(Mgr *mgr, Item *item)
{
    mgr->x31714 = item;
    item->flags |= 0x40000;
}
