/* UNIT 0x00158618 0x00158630 */
typedef struct Entry {
    int x0;
    int x4;
} Entry;

extern Entry D_0026E708[];

Entry *func_00158618(int id)
{
    return &D_0026E708[id >> 5];
}
