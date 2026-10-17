/* UNIT 0x00193478 0x00193498 */
struct Entry {
    char pad0[0x250];
};

extern struct Entry D_003F5690[];

int func_00193478(struct Entry *entry)
{
    return entry - D_003F5690;
}
