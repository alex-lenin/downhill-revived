/* UNIT 0x001BA368 0x001BA3A8 */
typedef struct Table {
    char pad0[0xA0C];
    int entries[1][10];
} Table;

int func_001BA368(Table *table, int id)
{
    id -= 9000;
    return table->entries[id / 10][id % 10];
}
