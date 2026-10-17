/* UNIT 0x0014DD78 0x0014DD90 */
typedef struct Entry {
    char data[30];
} Entry;

extern Entry D_0026E540[];

Entry *func_0014DD78(int i)
{
    return &D_0026E540[i];
}
