/* UNIT 0x00193498 0x001934B0 */
typedef struct Entry {
    char pad0[0x250];
} Entry;

extern Entry D_003F5690[];

Entry *func_00193498(int index)
{
    return &D_003F5690[index];
}
