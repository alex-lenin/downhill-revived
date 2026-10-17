/* UNIT 0x0011CF28 0x0011CF70 */
typedef struct Item {
    char pad0[0x270];
} Item;

extern void func_0011E528(Item *item);

void func_0011CF28(Item *items)
{
    int i;

    for (i = 0; i < 30; i++) {
        func_0011E528(&items[i]);
    }
}
