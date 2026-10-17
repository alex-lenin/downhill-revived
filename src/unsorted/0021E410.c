/* UNIT 0x0021E410 0x0021E428 */
struct Entry {
    char pad0[0x28];
    int x28;
    char pad2C[0x40 - 0x2C];
};

extern struct Entry D_0062F880[];

int func_0021E410(int index)
{
    struct Entry *entry;

    entry = &D_0062F880[index];
    return entry->x28;
}
