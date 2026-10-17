/* UNIT 0x00148260 0x001482B0 */
typedef struct Item {
    char pad0[0x380];
} Item;

extern void func_00148520(Item *);
extern void func_0014DB38();
extern void func_002182C8(void (*)());

void func_00148260(Item *items)
{
    int i;

    for (i = 0; i < 4; i++) {
        func_00148520(&items[i]);
    }
    func_002182C8(func_0014DB38);
}
