/* UNIT 0x00158650 0x00158668 */
typedef struct Entry {
    int x0;
    int x4;
} Entry;

extern Entry D_0026E8C8[];

Entry *func_00158650(int id)
{
    return &D_0026E8C8[id >> 5];
}
